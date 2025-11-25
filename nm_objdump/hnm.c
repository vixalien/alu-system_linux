#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <fcntl.h>
#include <gelf.h>
#include <unistd.h>
#include <string.h>

void list_symbols(const char *filename) {
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
    if (elf == NULL) {
        fprintf(stderr, "elf_begin() failed: %s.\n", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    if (elf_kind(elf) != ELF_K_ELF) {
        fprintf(stderr, "%s is not an ELF object.\n", filename);
        exit(EXIT_FAILURE);
    }

    size_t shstrndx;
    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        fprintf(stderr, "elf_getshdrstrndx() failed: %s.\n", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    Elf_Scn *scn = NULL;
    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        GElf_Shdr shdr;
        gelf_getshdr(scn, &shdr);

        if (shdr.sh_type == SHT_SYMTAB) {
            Elf_Data *data = elf_getdata(scn, NULL);
            int count = shdr.sh_size / shdr.sh_entsize;

            for (int i = 0; i < count; i++) {
                GElf_Sym sym;
                gelf_getsym(data, i, &sym);

                const char *name = elf_strptr(elf, shdr.sh_link, sym.st_name);
                if (name == NULL) {
                    name = "<no-name>";
                }

                printf("%08lx %c %s\n",
                       (unsigned long) sym.st_value,
                       (ELF32_ST_BIND(sym.st_info) == STB_GLOBAL) ? 'T' : 't',
                       name);
            }
        }
    }

    elf_end(elf);
    close(fd);
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

