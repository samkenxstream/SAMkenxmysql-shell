/*
 * Copyright (c) 2020, Oracle and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef MODULES_UTIL_DUMP_DUMP_OPTIONS_H_
#define MODULES_UTIL_DUMP_DUMP_OPTIONS_H_

#include <memory>
#include <string>

#include "mysqlshdk/include/scripting/types.h"
#include "mysqlshdk/libs/db/session.h"
#include "mysqlshdk/libs/oci/oci_options.h"
#include "mysqlshdk/libs/storage/compressed_file.h"
#include "mysqlshdk/libs/utils/nullable.h"
#include "mysqlshdk/libs/utils/version.h"

#include "modules/util/import_table/dialect.h"

namespace mysqlsh {
namespace dump {

class Dump_options {
 public:
  Dump_options() = delete;
  explicit Dump_options(const std::string &output_url);

  Dump_options(const Dump_options &) = default;
  Dump_options(Dump_options &&) = default;

  Dump_options &operator=(const Dump_options &) = default;
  Dump_options &operator=(Dump_options &&) = default;

  virtual ~Dump_options() = default;

  void validate() const;

  // setters
  void set_options(const shcore::Dictionary_t &options);

  void set_session(const std::shared_ptr<mysqlshdk::db::ISession> &session);

  // getters
  const std::string &output_url() const { return m_output_url; }

  const shcore::Dictionary_t &original_options() const { return m_options; }

  bool use_base64() const { return m_use_base64; }

  int64_t max_rate() const { return m_max_rate; }

  bool show_progress() const { return m_show_progress; }

  mysqlshdk::storage::Compression compression() const { return m_compression; }

  const std::shared_ptr<mysqlshdk::db::ISession> &session() const {
    return m_session;
  }

  const import_table::Dialect &dialect() const { return m_dialect; }

  const mysqlshdk::oci::Oci_options &oci_options() const {
    return m_oci_options;
  }

  const std::string &character_set() const { return m_character_set; }

  const mysqlshdk::utils::nullable<mysqlshdk::utils::Version>
      &mds_compatibility() const {
    return m_mds;
  }

  bool dump_schema_ddl() const { return dump_ddl() && !table_only(); }

  virtual bool split() const = 0;

  virtual uint64_t bytes_per_chunk() const = 0;

  virtual std::size_t threads() const = 0;

  virtual bool is_export_only() const = 0;

  virtual bool use_single_file() const = 0;

  virtual bool dump_ddl() const = 0;

  virtual bool table_only() const = 0;

  virtual bool dump_data() const = 0;

  virtual bool is_dry_run() const = 0;

  virtual bool consistent_dump() const = 0;

  virtual bool dump_events() const = 0;

  virtual bool dump_routines() const = 0;

  virtual bool dump_triggers() const = 0;

  virtual bool dump_users() const = 0;

  virtual bool use_timezone_utc() const = 0;

 protected:
  void set_compression(mysqlshdk::storage::Compression compression) {
    m_compression = compression;
  }

  void set_dialect(const import_table::Dialect &dialect) {
    m_dialect = dialect;
  }

  void set_mds_compatibility(
      const mysqlshdk::utils::nullable<mysqlshdk::utils::Version> &mds) {
    m_mds = mds;
  }

 private:
  virtual void unpack_options(shcore::Option_unpacker *unpacker) = 0;

  virtual void on_set_session(
      const std::shared_ptr<mysqlshdk::db::ISession> &session) = 0;

  virtual void validate_options() const = 0;

  virtual mysqlshdk::oci::Oci_options::Unpack_target oci_target() const = 0;

  // global session
  std::shared_ptr<mysqlshdk::db::ISession> m_session;

  // input arguments
  std::string m_output_url;
  shcore::Dictionary_t m_options;

  // not configurable
  bool m_use_base64 = true;

  // common options
  int64_t m_max_rate = 0;
  bool m_show_progress;
  mysqlshdk::storage::Compression m_compression =
      mysqlshdk::storage::Compression::ZSTD;
  mysqlshdk::oci::Oci_options m_oci_options;
  std::string m_character_set = "utf8mb4";

  // these options are unpacked elsewhere, but are here 'cause we're returning
  // a reference
  // currently used by exportTable()
  import_table::Dialect m_dialect;

  // currently used by dumpSchemas() and dumpInstance()
  mysqlshdk::utils::nullable<mysqlshdk::utils::Version> m_mds;
};

}  // namespace dump
}  // namespace mysqlsh

#endif  // MODULES_UTIL_DUMP_DUMP_OPTIONS_H_
