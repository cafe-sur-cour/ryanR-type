target_sources(${CLIENT_BIN_NAME} PRIVATE
    LeaderboardState.hpp
    LeaderboardState.cpp
)

set_source_files_properties(LeaderboardState.cpp PROPERTIES UNITY_BUILD OFF)