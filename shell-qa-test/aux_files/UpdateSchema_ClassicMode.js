session.sql("ALTER SCHEMA `sakila`  DEFAULT COLLATE utf8_general_ci ;");
session.sql("SELECT DEFAULT_COLLATION_NAME FROM information_schema.SCHEMATA WHERE SCHEMA_NAME = 'sakila' LIMIT 1;");