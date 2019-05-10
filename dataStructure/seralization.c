
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	int age;
	int height;
	char name[100];

}Student;

void Student_init(Student * s) {
	s->age = 0;
	s->height = 0;
	s->name[0] = 0;

}

void Student_init_withData(Student *s, const char * name , int age , int height) {
	s->age = age;
	s->height = height;
	strcpy(s->name, name);
}

void WriteInt(FILE *f, int v) {
	fwrite(&v, sizeof(v), 1,  f);
}


void WriteFile(FILE * f, Student *s) {

	WriteInt(f, s->age);
	WriteInt(f, s->height);
	int nameLen = strlen(s->name);
	fwrite(s->name, nameLen + 1, 1, f);

}

void ReadString(FILE *f, char* buff, int buffSize) {
	int size = 0;
	for (;;) {
		int c = fgetc(f);
		if (c == EOF) {
			break;
		}

		if (c == 0) {
			break;
		}


		if (size >= buffSize) {
			//string is too big
			assert(0);
			abort();
		}
		buff[size] = c;
		size++;

	}

	if (size >= buffSize) {
		//string is too big
		assert(0);
		abort();
	}
	buff[size] = 0;
}

void Readint(FILE *f , int *buff) {
	fread(buff, sizeof(*buff), 1, f);

}

void WriteList( Student * arr, int n) {
	FILE* f = fopen("StudentList", "wb");
	if (!f) {
		printf("cannot open file");
		assert(0);
		abort();
	}

	int ret = fwrite(&n, sizeof(n), 1, f);
	if (ret <= 0) {
		printf("cant write file");
		assert(0);
		abort();
	}

	for (int i = 0; i < n; i++) {
		WriteFile(f, &arr[i]);
	}

	fclose(f);
}

void ReadStudent(FILE *f, Student *buff) {
	if (buff&&f) {
		Readint(f, &buff->age);
		Readint(f, &buff->height);
		ReadString(f, buff->name, sizeof(buff->name));
	}
}

void ReadFileStudent(Student * buff,int buffSize) {

	FILE  * f = fopen("StudentList", "rb");

	if (!f) {
		printf("cannot open file");
		assert(0);
		abort();
	}

	int n;
	int Nsize = fread(&n, sizeof(buffSize), 1, f);
	if (Nsize <= 0) {
		printf("cant read");
		assert(0);
		abort();
	}

	for (int i = 0; i < n; i++) {
		
		ReadStudent(f, &buff[i]);
	}
	fclose(f);
}

void printList(Student * buff, int size) {

	printf("printing student list \n");
	for(int i = 0; i < size; i++) {
		Student *s = &buff[i];
		printf("	name:%s age:%d height:%d \n", s->name, s->age, s->height);
	}
	printf("finished printing \n\n");
}
int main() {
	Student list[3];
	
	printf("Putting three student into list  %d\n" , sizeof(int));
	Student_init_withData(&list[0], "tommy", 21, 180);
	Student_init_withData(&list[1], "Betty", 18, 172);
	Student_init_withData(&list[2], "Reggie", 18, 184);

	printList(list, 3);
	WriteList(list, 3);

	printList(list, 3);
	Student read[3];

	ReadFileStudent(read, 3);
	printList(read, 3);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
	


}