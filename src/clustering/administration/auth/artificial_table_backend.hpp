// Copyright 2010-2015 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_AUTH_BACKEND_HPP
#define CLUSTERING_ADMINISTRATION_AUTH_BACKEND_HPP

#include <string>
#include <vector>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "clustering/administration/metadata.hpp"
#include "clustering/table_manager/table_meta_client.hpp"
#include "rdb_protocol/artificial_table/caching_cfeed_backend.hpp"

namespace auth {

class artificial_table_backend_t :
    public caching_cfeed_artificial_table_backend_t
{
public:
    artificial_table_backend_t(
        boost::shared_ptr<semilattice_readwrite_view_t<auth_semilattice_metadata_t>>
            auth_semilattice_view,
        boost::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t>>
            cluster_semilattice_view,
        table_meta_client_t *table_meta_client,
        admin_identifier_format_t identifier_format);
    ~artificial_table_backend_t();

    std::string get_primary_key_name();

    bool read_all_rows_as_vector(
        signal_t *interruptor,
        std::vector<ql::datum_t> *rows_out,
        admin_err_t *error_out);

    bool read_row(
        ql::datum_t primary_key,
        signal_t *interruptor,
        ql::datum_t *row_out,
        admin_err_t *error_out);

    bool write_row(
        ql::datum_t primary_key,
        bool pkey_was_autogenerated,
        ql::datum_t *new_value_inout,
        signal_t *interruptor,
        admin_err_t *error_out);

private:
    boost::shared_ptr<semilattice_readwrite_view_t<auth_semilattice_metadata_t>>
        m_auth_semilattice_view;
    boost::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t>>
        m_cluster_semilattice_view;
    table_meta_client_t *m_table_meta_client;
    admin_identifier_format_t m_identifier_format;
};

}  // namespace auth

#endif  // CLUSTERING_ADMINISTRATION_AUTH_BACKEND_HPP
