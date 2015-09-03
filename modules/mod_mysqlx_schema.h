/*
 * Copyright (c) 2014, 2015, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

// Interactive DB access module
// (the one exposed as the db variable in the shell)

#ifndef _MOD_SCHEMA_H_
#define _MOD_SCHEMA_H_

#include "mod_common.h"
#include "base_database_object.h"
#include "shellcore/types.h"
#include "shellcore/types_cpp.h"

#include "mysqlx_crud.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/weak_ptr.hpp>

namespace shcore
{
  class Proxy_object;
};

namespace mysh
{
  namespace mysqlx
  {
    class BaseSession;
    class Table;
    class Collection;

    /**
    * Represents a Schema as retrived from a session created using the X Protocol.
    * \todo Implement and Document getCollectionAsTable()
    */
    class SHCORE_PUBLIC Schema : public DatabaseObject, public boost::enable_shared_from_this<Schema>
    {
    public:
      Schema(boost::shared_ptr<BaseSession> owner, const std::string &name);
      Schema(boost::shared_ptr<const BaseSession> owner, const std::string &name);
      ~Schema();

      virtual std::string class_name() const { return "Schema"; };

      virtual std::vector<std::string> get_members() const;

      virtual shcore::Value get_member(const std::string &prop) const;

      void cache_table_objects();

      friend class Table;
      friend class Collection;
      friend class View;
#ifdef DOXYGEN
      /**
      * Returns a list of tables as a map, for each entry being the key, an string, the table name and the data the Table object.
      * \sa getTables(), getViews(), getCollections()
      */
      Map tables;

      /**
      * Returns a list of tables as a map, for each entry being the key, an string, the table name and the data the Table object.
      * \sa tables
      * \return the map with the tables.
      */
      Map getTables()
      {}

      /**
      * Returns a list of views as a map, for each entry being the key, an string, the view name and the data the View object.
      * \sa getTables(), getViews(), getCollections()
      */
      Map views;

      /**
      * Returns a list of views as a map, for each entry being the key, an string, the view name and the data the View object.
      * \sa getTables(), getViews(), getCollections()
      * \return the map with the views.
      */
      Map getViews()
      {}

      /**
      * Returns a list of collections as a map, for each entry being the key, an string, the collection name and the data the CollectionRef object.
      * \sa getTables(), getViews(), getCollections()
      */
      Map collections;

      /**
      * Returns a list of collections as a map, for each entry being the key, an string, the collection name and the data the CollectionRef object.
      * \sa getTables(), getViews(), getCollections()
      * \return the map of collections.
      */
      Map getCollections();

      Table getTable(String name);
      View getView(String name);
      Collection getCollection(String name);
      Collection createCollection(String name);
#endif
    private:
      shcore::Value _load_object(const std::string& name, const std::string& type = "") const;
      boost::shared_ptr< ::mysqlx::Schema> _schema_impl;
      boost::shared_ptr<shcore::Value::Map_type> _tables;
      boost::shared_ptr<shcore::Value::Map_type> _collections;
      boost::shared_ptr<shcore::Value::Map_type> _views;

      shcore::Value find_in_collection(const std::string& name, boost::shared_ptr<shcore::Value::Map_type>source) const;
      shcore::Value getTable(const shcore::Argument_list &args);
      shcore::Value getCollection(const shcore::Argument_list &args);
      shcore::Value getView(const shcore::Argument_list &args);

      shcore::Value createCollection(const shcore::Argument_list &args);

      void init();
    };
  }
}

#endif
