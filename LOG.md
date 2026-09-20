08/11/2026-1: wrote the initial scripts for CMakeLists.txt, ran main, wrote clang-format, and checked versions were properly set for the future
08/12/2026-1: Added apt system packages, will use vcpkg for reproducible builds in a laer phase of this project
08/21/2026-1: Push everything to github for work on another computer
08/23/2026-1: Try-Catch block added to perform a TTLS handshake to coinbase, failed for now but progress made in the attempt, also modified cmaklists
08/23/2026-2: Fixed SSL websocket and esatblish a connection with coinbase
08/26/2026-1: Subscribed and opened the firehose, printing live trades now of BTC-USD
08/26/2026-2: Started on adding a parser for the string values coming from the exchange feed, working on making a function that prevents floating point inaccuracy
09/01/2026-1: Finished Parsing the string values over several days of work and learning more about beast, loop proprly collects values now
09/03/2026-1: Simplified the price formatting function, finished overall on the first milestone of reading and parsing values, now I can start on processing the order book
09/04/2026-1: Documented the functions for my own sanity, created a setup file for any future contributers and easier setup on a new machine, updated the README
09/04/2026-2: Started on order book class, added new file "order-book.h" and created 7 new function prototypes
09/09/2026-1: Created empty placeholder functions for my orderbook class
09/16/2026-1: Moved declarations to types.hpp, left a small mess between my files and needs fixing before the project runs again
09/16/2026-2: Rewrote my cmaketxts to include new files and link them to the library properly, still needs a few fixesm but it runs now
09/16/2026-3: Fixed Cmakelists.txt and organized it, now the libraries cleanly link to the main executable
09/20/2026-1: Wrote my test cases and successfuly got a testing build running, now I can write the functions
09/20/2026-2: Filled in all my functions, iteratively tested them until all test cases passed for the order book class