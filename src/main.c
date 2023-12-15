// jgabaut @ github.com/jgabaut
// SPDX-License-Identifier: GPL-3.0-only
/*
    Copyright (C) 2023  jgabaut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <koliseo.h>
#include "anvil__kls_hasher.h"

uint64_t hash(const char* string, size_t size)
{
    uint64_t hash = 5381;
    for (size_t i = 0; i < size-1; i++) {
        hash = (((hash << 5) + hash) + string[i]);
    }
    return hash;
}

void usage(const char* prog)
{
    printf("Usage:    %s [token]\n", prog);
}

#define TABLE_SIZE 10000

typedef uint64_t (hash_f) (const char*, size_t);

typedef struct ht_entry {
    const char* key;
    size_t keylen;
    void* obj;
    struct ht_entry* next;
} ht_entry;

typedef struct _hash_table {
    size_t size;
    hash_f *hash;
    ht_entry **pairs;
} hash_table;

void hash_table_print(hash_table* ht)
{
    for (size_t i=0; i < ht->size-1; i++) {
        if (ht->pairs[i] == NULL) {
            continue;
        } else {
            ht_entry* tmp = ht->pairs[i];
            while (tmp != NULL) {
                printf("Entry -> (key: \"" Kstr_Fmt "\", val: \"%p\")\n", Kstr_Arg(kstr_new(tmp->key, tmp->keylen)), tmp->obj);
                tmp = tmp->next;
            }
        }
    }
}

hash_table* hash_table_create(size_t size, hash_f * hash, Koliseo* kls)
{
    hash_table* ht = KLS_PUSH(kls,hash_table,1);
    ht->size = size;
    ht->hash = hash;
    ht->pairs = KLS_PUSH(kls,ht_entry*, ht->size);
    return ht;
}

size_t hash_table_index(hash_table* ht, const char* key, size_t key_size)
{
    size_t result = (ht->hash(key, key_size) % ht->size);
    return result;
}

void* hash_table_lookup(hash_table* ht, const char* key, size_t key_size)
{
    if (key == NULL || ht == NULL) return NULL;
    size_t index = hash_table_index(ht, key, key_size);

    ht_entry* tmp = ht->pairs[index];
    while (tmp != NULL && memcmp(tmp->key, key, key_size) != 0) {
        tmp = tmp->next;
    }

    if (tmp == NULL) return NULL;
    return tmp->obj;
}

bool hash_table_insert(hash_table* ht, const char *key, size_t key_size, void* obj, Koliseo* kls)
{
    if (key == NULL || obj == NULL) return false;
    size_t index = hash_table_index(ht, key, key_size);
    ht_entry* e = KLS_PUSH(kls,ht_entry,1);
    e->obj = obj;
    e->key = KLS_PUSH(kls,char,key_size);
    e->key = memcpy((void*)e->key, key, key_size);
    e->keylen = key_size;

    ht_entry* match = NULL;
    if ((match = hash_table_lookup(ht, key, key_size)) != NULL) {
        fprintf(stderr,"%s():    Collision on {" Kstr_Fmt "} -> [%lu]\n", __func__, Kstr_Arg(kstr_new(key, key_size)), index);
        //e->next = NULL;
        //match->next = e;
        return false;
    } else {
        e->next = ht->pairs[index];
        ht->pairs[index] = e;
    }
    return true;
}

int main(int argc, char** argv)
{
    printf("%s v%s\n", argv[0], get_ANVIL__VERSION__());
    printf("KLS API LVL:    {%i}\n", int_koliseo_version());
    printf("%s reference date: {%s}\n", argv[0], get_ANVIL__VERSION__DATE__());
    Koliseo* k = kls_new(KLS_DEFAULT_SIZE*8);

    int foo = 42;
    char buf[100] = {0};
    hash_table* map = hash_table_create(TABLE_SIZE, &hash, k);
    int res = -1;
    if (argc < 2) {
        printf("Enter string (CTRL-D to quit):\n");
        while ((res = scanf("%s", buf)) == 1) {
            uint64_t h = hash(buf, sizeof(buf)) % TABLE_SIZE;
            //printf("Hash:    {%lu}\n", h);
            if (!hash_table_insert(map, buf, strlen(buf), &foo, k)) {
                fprintf(stderr,"Failed insert for {%s} -> {%lu}\n", buf, h);
                kls_free(k);
                return 1;
            }
        }
        hash_table_print(map);
        kls_free(k);
        return 0;
    } else if (argc == 2) {
        printf("[DEBUG] File:    {%s}\n", argv[1]);
        Kstr* str = KLS_GULP_FILE_KSTR(k,argv[1]);
        if (!str) {
            fprintf(stderr, "Failed KLS_GULP_FILE_KSTR()\n");
            kls_free(k);
            return 1;
        }
        Kstr s = *str;
        Kstr tkn = KSTR_NULL;
        //printf("[DEBUG] Kstr:    {" Kstr_Fmt "}\n", Kstr_Arg(s));
        while (kstr_try_token(&s, ' ', &tkn)) {
            //printf("[DEBUG] Token: {" Kstr_Fmt "}\n", Kstr_Arg(tkn));
            Kstr trimmed = kstr_trim(tkn);
            //printf("[DEBUG] Trimmed: {" Kstr_Fmt "}\n", Kstr_Arg(trimmed));
            if (trimmed.len > 0) {
                uint64_t h = hash(trimmed.data, trimmed.len) % TABLE_SIZE;
                if (!hash_table_insert(map, trimmed.data, trimmed.len, &foo, k)) {
                    fprintf(stderr,"Failed insert for {" Kstr_Fmt "} -> {%lu}\n", Kstr_Arg(trimmed), h);
                    //kls_free(k);
                    //return 1;
                }
                //printf("Hash:    {%lu}\n", h);
            } else {
                continue;
            }
        }
        hash_table_print(map);
        kls_free(k);
        return 0;
    } else {
        usage(argv[0]);
        kls_free(k);
        return 1;
    }
}
