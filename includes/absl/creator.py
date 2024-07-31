from os.path import exists, join

if __name__ == "__main__":
    include_headers = {
        "flat_hash_map": "container/flat_hash_map.h",
        "flat_hash_set": "container/flat_hash_set.h",
        "node_hash_map": "container/node_hash_map.h",
        "node_hash_set": "container/node_hash_set.h",
        "btree_map": "container/btree_map.h",
        "btree_set": "container/btree_set.h",
        "hash": "hash/hash.h",
        "random": "random/random.h",
        "str_strip": "strings/strip.h",
        "str_split": "strings/str_split.h",
        "str_replace": "strings/str_replace.h",
        "str_join": "strings/str_join.h",
        "str_cat": "strings/str_cat.h",
        "str_numbers": "strings/numbers.h",
    }

    for name, filepath in include_headers.items():
        header_filename = join("includes", "absl", f"{name}.hpp")

        if exists(header_filename):
            continue

        with open(header_filename, "w") as header_file:
            header_file.write(
                f"""#pragma once

#include "../../third-party/abseil-cpp/absl/{filepath}"
"""
            )
