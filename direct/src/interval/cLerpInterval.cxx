/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file cLerpInterval.cxx
 * @author drose
 * @date 2002-08-27
 */

#include "cLerpInterval.h"
#include "string_utils.h"

TypeHandle CLerpInterval::_type_handle;

/**
 * Returns the BlendType enumerated value corresponding to the indicated
 * string, or BT_invalid if the string doesn't match anything.
 */
CLerpInterval::BlendType CLerpInterval::
string_blend_type(std::string_view blend_type) {
  if (blend_type == "easeIn") {
    return BT_ease_in;
  } else if (blend_type == "easeOut") {
    return BT_ease_out;
  } else if (blend_type == "easeInOut") {
    return BT_ease_in_out;
  } else if (blend_type == "noBlend") {
    return BT_no_blend;
  } else if (blend_type == "quintIn") {
    return BT_quint_in;
  } else if (blend_type == "quintOut") {
    return BT_quint_out;
  } else if (blend_type == "quintInOut") {
    return BT_quint_in_out;
  } else if (blend_type == "circIn") {
    return BT_circ_in;
  } else if (blend_type == "circOut") {
    return BT_circ_out;
  } else if (blend_type == "circInOut") {
    return BT_circ_in_out;
  } else if (blend_type == "sineIn") {
    return BT_sine_in;
  } else if (blend_type == "sineOut") {
    return BT_sine_out;
  } else if (blend_type == "sineInOut") {
    return BT_sine_in_out;
  } else if (blend_type == "cubicIn") {
    return BT_cubic_in;
  } else if (blend_type == "cubicOut") {
    return BT_cubic_out;
  } else if (blend_type == "cubicInOut") {
    return BT_cubic_in_out;
  } else if (blend_type == "backIn") {
    return BT_back_in;
  } else if (blend_type == "backOut") {
    return BT_back_out;
  } else if (blend_type == "backInOut") {
    return BT_back_in_out;
  } else {
    return BT_invalid;
  }
}


/**
 * Given a t value in the range [0, get_duration()], returns the corresponding
 * delta value clamped to the range [0, 1], after scaling by duration and
 * applying the blend type.
 */
double CLerpInterval::
compute_delta(double t) const {
  double duration = get_duration();
  if (duration == 0.0) {
    // If duration is 0, the lerp works as a set.  Thus, the delta is always
    // 1.0, the terminating value.
    return 1.0;
  }
  t /= duration;
  t = std::min(std::max(t, 0.0), 1.0);

  switch (_blend_type) {
  case BT_ease_in:
    {
      double t2 = t * t;
      return ((3.0 * t2) - (t2 * t)) * 0.5;
    }

  case BT_ease_out:
    {
      double t2 = t * t;
      return ((3.0 * t) - (t2 * t)) * 0.5;
    }

  case BT_ease_in_out:
    {
      double t2 = t * t;
      return (3.0 * t2) - (2.0 * t * t2);
    }

  case BT_quint_in:
    return t * t * t * t * t;  // Quintic easing in

  case BT_quint_out:
    t -= 1.0;
    return 1 + t * t * t * t * t;  // Quintic easing out

  case BT_quint_in_out:
    if (t < 0.5) {
      return 16 * t * t * t * t * t;
    } else {
      t -= 1.0;
      return 1 + 16 * t * t * t * t * t;
    }

  case BT_circ_in:
    return 1 - std::sqrt(1 - t * t);  // Circular easing in

  case BT_circ_out:
    t -= 1.0;
    return std::sqrt(1 - t * t);  // Circular easing out

  case BT_circ_in_out:
    if (t < 0.5) {
      return (1 - std::sqrt(1 - (2 * t) * (2 * t))) * 0.5;
    } else {
      t = (2 * t) - 2;
      return (std::sqrt(1 - t * t) + 1) * 0.5;
    }

  case BT_sine_in:
    return 1 - std::cos((t * M_PI) / 2);  // Sine easing in

  case BT_sine_out:
    return std::sin((t * M_PI) / 2);  // Sine easing out

  case BT_sine_in_out:
    return -(std::cos(M_PI * t) - 1) / 2;  // Sine easing in-out

  case BT_cubic_in:
    return t * t * t;  // Cubic easing in

  case BT_cubic_out:
    t -= 1.0;
    return t * t * t + 1;  // Cubic easing out

  case BT_cubic_in_out:
    if (t < 0.5) {
      return 4 * t * t * t;
    } else {
      t -= 1.0;
      return 4 * t * t * t + 1;
    }

  case BT_back_in:
    {
      double s = 1.70158;
      return t * t * ((s + 1) * t - s);  // Back easing in
    }

  case BT_back_out:
    {
      double s = 1.70158;
      t -= 1.0;
      return t * t * ((s + 1) * t + s) + 1;  // Back easing out
    }

  case BT_back_in_out:
    {
      double s = 1.70158 * 1.525;
      if (t < 0.5) {
        t *= 2;
        return 0.5 * (t * t * ((s + 1) * t - s));
      } else {
        t = (t * 2) - 2;
        return 0.5 * (t * t * ((s + 1) * t + s) + 2);
      }
    }

  default:
    return t;
  }
}
