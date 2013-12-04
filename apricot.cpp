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
    
    // get the SQLite3 DBD driver
    apr_dbd_driver_t const *driver;
    rv = apr_dbd_get_driver(pool, "sqlite3", &driver);
    exit_on_err(rv, "apr_dbd_get_driver (sqlite3)");
    
    // get the MySQL DBD driver
    rv = apr_dbd_get_driver(pool, "mysql", &driver);
    exit_on_err(rv, "apr_dbd_get_driver (mysql)");
    
    printf("hello world\n");
    exit(0);
}
