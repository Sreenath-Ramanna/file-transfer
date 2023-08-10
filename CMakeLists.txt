cmake_minimum_required(VERSION 3.0.0)
project(fileTransfer VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(ft_server file_hash.c 
                main_server.c 
                file_transfer_helpers.c 
                file_manager.c 
                file_transfer_server.c 
                simple_base64.c 
                client_handler.c 
                utils.c)
target_link_libraries(ft_server -lpthread -lssl -lcrypto)

add_executable(ft_client file_transfer_client.c
                  file_hash.c 
                  file_transfer_helpers.c 
                  simple_base64.c 
                  utils.c )
target_link_libraries(ft_client -lssl -lcrypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
