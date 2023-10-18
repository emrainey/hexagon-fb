/*******************************************************************************
 * COPYRIGHT (C) 2003, ERIK RAINEY.
 *
 * This source code has not yet been approved for release under any license
 * and as such has no warranty or fitness of purpose associated with it. Use at
 * your own discretion.
 *
 * @author Erik Rainey
 * @version 1.0
 ******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <cstdint>
#include <type_traits>

template <typename Enum>
constexpr typename std::underlying_type<Enum>::type to_underlying(Enum e) noexcept {
    return static_cast<typename std::underlying_type<Enum>::type>(e);
}

template <typename ENUM_TYPE, typename MASK_TYPE>
class Mask {
public:
    Mask() { m_mask = 0; }
    Mask(MASK_TYPE mask) { m_mask = mask; }
    ~Mask() {}
    void Set(ENUM_TYPE flag) { m_mask |= (1 << to_underlying(flag)); }
    void Clear(ENUM_TYPE flag) { m_mask &= ~(1 << to_underlying(flag)); }
    bool IsSet(ENUM_TYPE flag) const { return (m_mask & (1 << to_underlying(flag))) != 0; }
    MASK_TYPE Get(void) const { return m_mask; }
    MASK_TYPE AsMask(ENUM_TYPE flag) const { return (1 << to_underlying(flag)); }

    Mask& operator|=(ENUM_TYPE flag) {
        Set(flag);
        return *this;
    }
    Mask& operator^=(ENUM_TYPE flag) {
        m_mask ^= (1 << to_underlying(flag));
        return *this;
    }
    Mask& operator=(ENUM_TYPE flag) {
        m_mask = (1 << to_underlying(flag));
        return *this;
    }

protected:
    MASK_TYPE m_mask;
};

// Super Class of all debugging classes.
class Debug {
public:
    // Is debug on or off?
    bool state;

    enum class Subsystem : std::uint32_t {
        Error,
        Info,
        Selection,
        Menu,
        Models,
        Input,
        Internal,
        Platform,
        Trace,
    };
    using MaskType = std::uint32_t;

    // class constructor
    Debug();
    Debug(bool state);

    // class destructor
    ~Debug();

    // typical info popup call
    void info(Subsystem subsystem, char const* const format, ...);

    // Wrapper to most flush operations
    void flush(void);

    /**
     * Disables a susbystem from printing debug. See #defines.
     */
    void disable(Subsystem subsystem);

    /**
     * Enables a subsystem for debugging. See bit field #defines.
     */
    void enable(Subsystem subsystem);

    void operator=(MaskType mask) { subsystems = mask; }
    void operator^=(Subsystem const& sub) { subsystems ^= sub; }
    void operator|=(Subsystem const& sub) { subsystems |= sub; }

protected:
    // which Subsystem are enabled?
    Mask<Subsystem, MaskType> subsystems;
};

#ifndef EXCLUDE_EXTERNS
extern Debug debug;
#endif

#endif  // DEBUG_H
