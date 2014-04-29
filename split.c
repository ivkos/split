#include <stdio.h>
#include <math.h>   // ceil()
#include <stdlib.h> // exit()

int main(int argc, char *argv[]) {
	char part_filename[200];
	FILE *ptr_source, *ptr_part;
	size_t parts, i;
	unsigned long int part_size, written_bytes;
	int byte;

	if (argc != 3) {
		printf("Usage: %s <maximum part size in MiB> <file to split>\n", argv[0]);
		exit(1);
	}

	if ((ptr_source = fopen(argv[2], "rb")) == NULL) {
		perror("Could not open file to split");
		exit(1);
	}

	fseek(ptr_source, 0, SEEK_END);
	part_size = atol(argv[1]) * 1024 * 1024;
	if (part_size == 0) {
		printf("Invalid part size: %s\n", argv[1]);
		exit(1);
	}
	parts = (size_t) ceil((double) ftell(ptr_source) / part_size);
	rewind(ptr_source);

	printf("File will be split into %u parts.\n", parts);

	for (i = 0; i < parts; i++) {
		printf("\nWriting part%02d... ", i);
		sprintf(part_filename, "%s.part%02d", argv[2], i);
		
		if ((ptr_part = fopen(part_filename, "wb")) == NULL) {
			perror("Could not open part for writing");
			exit(1);
		}

		for (written_bytes = 0; written_bytes < part_size &&
		    (byte = fgetc(ptr_source)) != EOF; written_bytes++)
		{
			fputc(byte, ptr_part);
		}

		printf("(%lu bytes)", written_bytes);
		fclose(ptr_part);
	}

	printf("\n\nDone!\n");
	fclose(ptr_source);

	return 0;
}
