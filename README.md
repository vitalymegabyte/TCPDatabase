# TCPDatabase

This is simple database, working on standard socket API. To build server, use g++ key `-pthread`.

Run server: `./server -p [PORT]`

Run client: `./client -p [PORT] -h [HOST]`

## Queries
Queries are similar with terminal options, that makes process of using easier.

To make query use this pattern: `[QUERYTYPE] [KEY] [VALUE]`
`[KEY]` should be a number
`[VALUE]` is not necessary in some types of queries.

`[QUERYTYPE]` has different variaties:

- `-a` — add key and value into database
- `-g` — get value from database by key
- `-c` — get information about existing of the key (TRUE for existing key, FALSE in other case)
