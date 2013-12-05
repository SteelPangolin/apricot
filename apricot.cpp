#include <stdlib.h>
#include <stdio.h>

#include <apr_errno.h>
#include <apr_general.h>
#include <apr_pools.h>
#include <apr_dbd.h>

void exit_on_err(apr_status_t rv, const char * tag)
{
    char err_buf[256];
    
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, err_buf, sizeof(err_buf));
        printf("%s failed: %s\n", tag, err_buf);
        exit(1);
    }
}

void print_dbd_err(const apr_dbd_driver_t *driver, apr_dbd_t * cxn, int dberr)
{
    if (dberr != 0) {
        printf("DBD error: %s\n", apr_dbd_error(driver, cxn, dberr));
    }
}

int main(int argc, const char * const * argv)
{
    apr_status_t rv;
    
    // start up APR
    rv = apr_app_initialize(&argc, &argv, NULL);
    exit_on_err(rv, "apr_app_initialize");
    
    // register APR final cleanup function
    atexit(apr_terminate);
    
    // create a memory pool (will be cleaned up by apr_terminate)
    apr_pool_t *pool;
    rv = apr_pool_create(&pool, NULL);
    exit_on_err(rv, "apr_pool_create");
    
    // start up DBD, the APR SQL abstraction layer
    rv = apr_dbd_init(pool);
    exit_on_err(rv, "apr_dbd_init");
    
    // DB config for MySQL
    const char *driver_name = "mysql";
    const char *params =
        "host=127.0.0.1"       "\n"
        "user=mod_auth_openid" "\n"
        "pass=abracadabra"     "\n"
        "dbname=openid";
    
    // get the DBD driver
    apr_dbd_driver_t const *driver;
    rv = apr_dbd_get_driver(pool, driver_name, &driver);
    exit_on_err(rv, "apr_dbd_get_driver");
    
    // open a SQL connection
    apr_dbd_t *cxn;
    const char *connect_err;
    rv = apr_dbd_open_ex(driver, pool, params, &cxn, &connect_err);
    if (rv != APR_SUCCESS) {
        printf("apr_dbd_open_ex error: %s\n", connect_err);
    }
    exit_on_err(rv, "apr_dbd_open_ex");
    
    // show some MySQL info
    int dberr;
    const char *query = "select CURRENT_USER() as user, VERSION() as version, DATABASE() as dbname";
    apr_dbd_results_t *results;
    dberr = apr_dbd_select(driver, pool, cxn, &results, query, 0 /* don't need random row access */);
    print_dbd_err(driver, cxn, dberr);
    
    int n_cols = apr_dbd_num_cols(driver, results);
    apr_dbd_row_t *row;
    while (!apr_dbd_get_row(driver, pool, results, &row, -1 /* next row */)) {
        for (int col = 0; col < n_cols; col++) {
            const char *name  = apr_dbd_get_name (driver, results, col);
            const char *value = apr_dbd_get_entry(driver, row,     col);
            printf("%s: %s\n", name, value);
        }
        printf("\n");
    }
    
    // close the SQL connection
    rv = apr_dbd_close(driver, cxn);
    exit_on_err(rv, "apr_dbd_close");
    
    printf("hello world\n");
    exit(0);
}
