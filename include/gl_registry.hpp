#pragma once
#ifndef BE_BGLGEN_GL_REGISTRY_HPP_
#define BE_BGLGEN_GL_REGISTRY_HPP_

#include <be/sqlite/db.hpp>
#include <be/core/filesystem.hpp>

namespace be::bglgen {

//////////////////////////////////////////////////////////////////////////////
sqlite::Db init_registry(const Path& xml_path, const Path& db_path, bool force_rebuild);

} // be::bglgen

#endif
