
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <raymath.h>

// -----------------------------------------------------------------------------
// Types(Defs)
// -----------------------------------------------------------------------------
typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;

typedef float       f32;
typedef double      f64;

typedef Vector2     v2;

#define Rect(x, y, w, h) ((Rectangle){ x, y, w, h })
#define Vec2(x, y) ((Vector2){x, y})

#endif
