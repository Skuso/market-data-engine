08/11/2026-1: wrote the initial scripts for CMakeLists.txt, ran main, wrote clang-format, and checked versions were properly set for the future
08/12/2026-1: Added apt system packages, will use vcpkg for reproducible builds in a laer phase of this project
08/21/2026-1: Push everything to github for work on another computer
08/23/2026-1: Try-Catch block added to perform a TTLS handshake to coinbase, failed for now but progress made in the attempt, also modified cmaklists
08/23/2026-2: Fixed SSL websocket and esatblish a connection with coinbase
08/26/2026-1: Subscribed and opened the firehose, printing live trades now of BTC-USD
08/26/2026-2: Started on adding a parser for the string values coming from the exchange feed, working on making a function that prevents floating point inaccuracy
09/01/2026-1: Finished Parsing the string values over several days of work and learning more about beast, loop proprly collects values now