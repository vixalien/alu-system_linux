#include <stdio.h>
#include <stdlib.h>
#include <bfd.h>

void list_symbols(const char *filename) {
    bfd *abfd;
    char **matching;
    
    bfd_init();
    abfd = bfd_openr(filename, NULL);
    if (!abfd) {
        bfd_perror("bfd_openr");
        return;
    }

    if (!bfd_check_format_matches(abfd, bfd_object, &matching)) {
        bfd_perror("bfd_check_format_matches");
        bfd_close(abfd);
        return;
    }

    long storage_needed = bfd_get_symtab_upper_bound(abfd);
    if (storage_needed < 0) {
        bfd_perror("bfd_get_symtab_upper_bound");
        bfd_close(abfd);
        return;
    }

    asymbol **symbol_table = (asymbol **) malloc(storage_needed);
    long number_of_symbols = bfd_canonicalize_symtab(abfd, symbol_table);
    if (number_of_symbols < 0) {
        bfd_perror("bfd_canonicalize_symtab");
        bfd_close(abfd);
        free(symbol_table);
        return;
    }

    for (long i = 0; i < number_of_symbols; i++) {
        printf("%08lx %c %s\n",
            (unsigned long) bfd_asymbol_value(symbol_table[i]),
            bfd_get_symbol_leading_char(abfd, symbol_table[i]) ? 'T' : 't',
            bfd_asymbol_name(symbol_table[i]));
    }

    bfd_close(abfd);
    free(symbol_table);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        list_symbols(argv[i]);
    }

    return 0;
}
