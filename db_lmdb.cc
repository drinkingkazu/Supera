#include "db_lmdb.h"
#include <sys/stat.h>
#include <string>
#include <iostream>

namespace db {

const size_t LMDB_MAP_SIZE = 1099511627776;  // 1 TB

void LMDB::Open(const std::string& source, Mode mode) {
  mdb_env_create(&mdb_env_);
  mdb_env_set_mapsize(mdb_env_, LMDB_MAP_SIZE);
  if (mode == NEW) {
    if ( mkdir(source.c_str(), 0744)!=0 )
      std::cout << "[LMDB] mkdir " << source << "failed" << std::endl;
    //CHECK_EQ(mkdir(source.c_str(), 0744), 0) << "mkdir " << source << "failed";
  }
  int flags = 0;
  if (mode == READ) {
    flags = MDB_RDONLY | MDB_NOTLS;
  }
  int rc = mdb_env_open(mdb_env_, source.c_str(), flags, 0664);
#ifndef ALLOW_LMDB_NOLOCK
  //MDB_CHECK(rc);
#else
  if (rc == EACCES) {
    std::cout << "Permission denied. Trying with MDB_NOLOCK ...";
    // Close and re-open environment handle
    mdb_env_close(mdb_env_);
    mdb_env_create(&mdb_env_);
    // Try again with MDB_NOLOCK
    flags |= MDB_NOLOCK;
    mdb_env_open(mdb_env_, source.c_str(), flags, 0664);
  } else {
    //MDB_CHECK(rc);
  }
#endif
  std::cout << "[LMDB] Opened database (status=" << rc << " " << mdb_strerror(rc) << ") " << source << std::endl;
}

LMDBCursor* LMDB::NewCursor() {
  MDB_txn* mdb_txn;
  MDB_cursor* mdb_cursor;
  mdb_txn_begin(mdb_env_, NULL, MDB_RDONLY, &mdb_txn);
  mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_);
  mdb_cursor_open(mdb_txn, mdb_dbi_, &mdb_cursor);
  // MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, MDB_RDONLY, &mdb_txn));
  // MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
  // MDB_CHECK(mdb_cursor_open(mdb_txn, mdb_dbi_, &mdb_cursor));
  return new LMDBCursor(mdb_txn, mdb_cursor);
}

LMDBTransaction* LMDB::NewTransaction() {
  MDB_txn* mdb_txn;
  mdb_txn_begin(mdb_env_, NULL, 0, &mdb_txn);
  mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_);
  // MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, 0, &mdb_txn));
  // MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
  return new LMDBTransaction(&mdb_dbi_, mdb_txn);
}

void LMDBTransaction::Put(const std::string& key, const std::string& value) {
  MDB_val mdb_key, mdb_value;
  mdb_key.mv_data = const_cast<char*>(key.data());
  mdb_key.mv_size = key.size();
  mdb_value.mv_data = const_cast<char*>(value.data());
  mdb_value.mv_size = value.size();
  //MDB_CHECK(mdb_put(mdb_txn_, *mdb_dbi_, &mdb_key, &mdb_value, 0));
  mdb_put(mdb_txn_, *mdb_dbi_, &mdb_key, &mdb_value, 0);
}

}  // namespace db

