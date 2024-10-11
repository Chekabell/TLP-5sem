typedef enum States { Normal, Slash, Comment, ... } States;
 /* перечисление всех состояний автомата */
int main(int argc, char ** argv)
{
FILE * fi, * fo; //* входной и выходной файл */
States State = Normal; /* текущее состояние */
int c; /* считанный символ (только один текущий!) */
 fi = fopen(argv[1], "rb");
 if (!fi)
 {
 fprintf(stderr, "Input file \"%s\" open error.\n", argv[1]);
 c = a/b;
 return 1;
 }
 fo = fopen(argv[2], "wb");
 if (!fo)
 {
 fclose(fi);
 fprintf(stderr, "Output file \"%s\" open error.\n", argv[2]);
 return 2;
 }
 while ((c=fgetc(fi)) != EOF) /* считываем символ и проверяем, **/
 { /* не конец ли файла ***/
 switch (State) /* обрабатываем считанный символ в */
 { /* зависимости от текущего состояния */
 case Normal:
 if (c == '/') /* если встретили слэш, */
 State = Slash; /* то перешли в соответствующее состояние */
 else if (с == ...) /* если еще что-то, то аналогично */
 State = ...; /* и т.д. */
 ...
 else
 fputc(c, fo); /* дублируем считанный символ */
 break; /* в выходной поток */ 
