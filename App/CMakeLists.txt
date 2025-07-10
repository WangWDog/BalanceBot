# 自动收集所有 .c 文件
file(GLOB_RECURSE APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.c
)

# 添加 App 源文件到目标可执行文件
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${APP_SOURCES})

# 添加 App 子目录的头文件路径
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/motor
        ${CMAKE_CURRENT_SOURCE_DIR}/control_state
        ${CMAKE_CURRENT_SOURCE_DIR}/encoder
        ${CMAKE_CURRENT_SOURCE_DIR}/pid
        ${CMAKE_CURRENT_SOURCE_DIR}/mpu6050
        ${CMAKE_CURRENT_SOURCE_DIR}/imu_filter
        ${CMAKE_CURRENT_SOURCE_DIR}/balance_ctrl
        ${CMAKE_CURRENT_SOURCE_DIR}/ir_tracking
        ${CMAKE_CURRENT_SOURCE_DIR}/uart_bt
        ${CMAKE_CURRENT_SOURCE_DIR}/mode_switch
)