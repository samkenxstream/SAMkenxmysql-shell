/*
 * Copyright (c) 2014, 2017, Oracle and/or its affiliates. All rights reserved.
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

#include "shellcore/shell_jscript.h"
#include "scripting/jscript_context.h"
#include "shellcore/base_session.h"
#include "shellcore/interrupt_handler.h"
#include "modules/devapi/mod_mysqlx_session.h"
#include "mysqlshdk/include/shellcore/base_shell.h"

using namespace shcore;

Shell_javascript::Shell_javascript(Shell_core *shcore)
    : Shell_language(shcore),
      _js(new JScript_context(shcore->registry(), shcore->get_delegate())) {
}

void Shell_javascript::handle_input(std::string &code, Input_state &state,
    std::function<void(shcore::Value)> result_processor) {
  // Undefined to be returned in case of errors
  Value result;

  shcore::Interrupt_handler inth([this]() {
    abort();
    return true;
  });

  if (mysqlsh::Base_shell::options().interactive)
    result = _js->execute_interactive(code, state);
  else {
    try {
      result = _js->execute(code, _owner->get_input_source(), _owner->get_input_args());
    } catch (std::exception &exc) {
      _owner->print_error(exc.what());
    }
  }

  _last_handled = code;

  result_processor(result);
}

void Shell_javascript::set_global(const std::string &name, const Value &value) {
  _js->set_global(name, value);
}

void Shell_javascript::abort() noexcept {
  // Abort execution of JS code
  // To abort execution of MySQL query called during JS code, a separate
  // handler should be pushed into the stack in the code that performs the query
  log_info("User aborted JavaScript execution (^C)");
  _js->terminate();
}
