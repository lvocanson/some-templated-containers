#pragma once
#include "type_set.h"
#include <format>
#include <variant>

/* A state is a type with declared exit transitions. */
template <typename T>
concept declared_state = template_instance_of<typename T::exits, type_set>;

// Check if T can reach U
template<typename T, typename U>
concept reach = declared_state<T> && declared_state<U> && requires(T t)
{
	{ U::enter(t) } -> std::same_as<U>;
};

// Trait that check if T can reach all Ts...
template <typename T, typename... Ts>
struct can_reach_all : std::bool_constant<(reach<T, Ts> && ...)> {};

// Concept that check if T can reach all Ts...
template <typename T, typename... Ts>
concept reach_all = can_reach_all<T, Ts...>::value;

/* A state is verified if all its exits are actually enterables. */
template <typename T>
concept verified_state = declared_state<T> && T::exits::template apply<can_reach_all, T>::value;

template <typename...> struct are_verified_states;

template <typename T, typename... Ts>
struct are_verified_states<T, Ts...> : std::bool_constant<verified_state<T> && are_verified_states<Ts...>::value> {};

template <>
struct are_verified_states<> : std::true_type {};

template <typename... Ts>
concept verified_states = are_verified_states<Ts...>::value;

template <declared_state From, declared_state To>
struct state_transition_exception
	: std::logic_error
{
	state_transition_exception()
		: std::logic_error(std::format(
		From::exits::template contains<To>
		? "`{1}` does not implements the function `static {1} enter({0}&);`."
		: "`{1}` is not listed in `{0}' exits."
		, typeid(From).name(), typeid(To).name()))
	{
	}
};

// Explore all exits of a state recursively 
template <declared_state...> struct get_state_network;

// Foreach T to explore
template <declared_state T, declared_state... Ts>
struct get_state_network<T, Ts...>
{
	template <typename... Us>
	struct accumulated
	{
		using type = std::conditional_t<
			contained_in<T, Us...>,

			// Already accumulated
			typename get_state_network<Ts...>
			::template accumulated<Us...>::type,

			// Add transitions in get_state_network and add T in accumulated
			typename T::exits
			::template remove<T, Ts..., Us...>
			::template add<Ts...>
			::template apply<get_state_network>
			::template accumulated<T, Us...>::type>;
	};
};

// End recursion, give all accumulated
template <>
struct get_state_network<>
{
	template <typename... Us>
	struct accumulated
	{
		using type = type_set<Us...>;
	};
};

// Check if T is in the network of S
template <typename T, typename S>
concept networkable_from = declared_state<S> && get_state_network<S>::template accumulated<>::type::template contains<T>;

template <declared_state InitialState>
class inferred_state_machine
{
public:

	using initial_state = InitialState;
	using state_network = get_state_network<initial_state>::template accumulated<>::type;
	using variant_type = state_network::template apply<std::variant>;

	// Whether the state network is make of only verified states.
	static constexpr bool verified = state_network::template apply<are_verified_states>::value;

private:

	variant_type state_;

public:

	template <typename... Args>
	explicit inferred_state_machine(Args&&... args)
		: state_(std::in_place_type<initial_state>, std::forward<Args>(args)...)
	{
	}

	// Calls std::visit on the current state
	decltype(auto) visit(auto&& visitor)
	{
		return std::visit(std::forward<decltype(visitor)>(visitor), state_);
	}

	// Calls std::visit on the current state (const version)
	decltype(auto) visit(auto&& visitor) const
	{
		return std::visit(std::forward<decltype(visitor)>(visitor), state_);
	}

	template <typename T>
	bool is_state() const
	{
		return std::holds_alternative<T>(state_);
	}

	template <typename T>
	T* get_if() const
	{
		return get_if<T>();
	}

	template <declared_state T>
	bool can_transition() const
	{
		return visit([](auto& declared_state)
		{
			using state_t = std::decay_t<decltype(declared_state)>;
			return reach<state_t, T>;
		});
	}

	template <declared_state T>
	void transition()
	{
		visit([this](auto& declared_state)
		{
			using state_t = std::decay_t<decltype(declared_state)>;
			if constexpr (reach<state_t, T>)
			{
				state_.emplace<T>(T::enter(declared_state));
			}
			else
			{
				throw state_transition_exception<state_t, T>();
			}
		});
	}

	template <typename T>
	bool try_transition()
	{
		return visit([this](auto& declared_state) -> bool
		{
			using state_t = std::decay_t<decltype(declared_state)>;
			if constexpr (reach<state_t, T>)
			{
				state_.emplace<T>(T::enter(declared_state));
				return true;
			}
			return false;
		});
	}

	// Force transition to a type, using one of its constructors
	template <typename T, typename... Args>
	void force_transition(Args&&... args)
	{
		state_.emplace<T>(std::forward<Args>(args)...);
	}
};
