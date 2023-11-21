#include "luthor/error.hpp"

namespace luthor {

/******************************************************************************/
/*                                                                            */
/*                              ERROR - PUBLIC */
/*                                                                            */
/******************************************************************************/

error::error(const std::string message) : std::runtime_error(message) { }

/******************************************************************************/
/*                                                                            */
/*                         NOT IMPLEMENTED - PUBLIC */
/*                                                                            */
/******************************************************************************/

not_implemented_error::not_implemented_error()
    : error("tried to call a not implemented function") { }

}  // namespace luthor
