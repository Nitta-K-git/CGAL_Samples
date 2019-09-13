message("---pybind11 setting---")
message("must link libraries--------------------------------------------------------")
message("C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib")
message("---------------------------------------------------------------------------")

include_directories(C:/Users/nitta/AppData/Local/Programs/Python/Python35/include)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
# slots,signals,emitの代わりにQ_SLOTS,Q_SIGNALS,Q_EMITを使用すること

# target_link_libraries(${PROJECT_NAME} 
#    C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib
# )
