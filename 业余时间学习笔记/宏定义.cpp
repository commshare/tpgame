1,��ֹһ��ͷ�ļ����ظ����� 
#ifndef BODYDEF_H 
#define BODYDEF_H 
  //ͷ�ļ����� 
#endif 
2,�õ�ָ����ַ�ϵ�һ���ֽڻ��� 
#define  MEM_B( x )  ( *( (byte *) (x) ) ) 
#define  MEM_W( x )  ( *( (word *) (x) ) ) 
3,�õ�һ��field�ڽṹ��(struct)�е�ƫ���� 
#define FPOS( type, field ) ( (dword) &(( type *) 0)-> field )
4,�õ�һ���ṹ����field��ռ�õ��ֽ��� 
#define FSIZ( type, field ) sizeof( ((type *) 0)->field ) 
5,�õ�һ�������ĵ�ַ��word��ȣ� 
#define  B_PTR( var )  ( (byte *) (void *) &(var) ) 
#define  W_PTR( var )  ( (word *) (void *) &(var) ) 
6,��һ����ĸת��Ϊ��д 
#define  UPCASE( c ) ( ((c) >= ''a'' && (c) <= ''z'') ? ((c) - 0x20) : (c) ) 
7,�ж��ַ��ǲ���10��ֵ������ 
#define  DECCHK( c ) ((c) >= ''0'' && (c) <= ''9'') 
8,�ж��ַ��ǲ���16��ֵ������ 
#define  HEXCHK( c ) ( ((c) >= ''0'' && (c) <= ''9'') ||((c) >= ''A'' && (c) <= ''F'') ||((c) >= ''a'' && (c) <= ''f'') ) 
9,��ֹ�����һ������ 
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val)) 
10,��������Ԫ�صĸ��� 
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) ) 
11,ʹ��һЩ����ٵ��� 
ANSI��׼˵�������Ԥ����ĺ����������ǣ� 
_LINE_ (�����»���)����Ӧ%d
_FILE_     ��Ӧ%s
_DATE_   ��Ӧ%s
_TIME_    ��Ӧ%s
_STDC_ 

 
����"#"��"##"���÷�
����ʹ��#�Ѻ������Ϊһ���ַ���,��##�����������������һ��.
#define STR(s)     #s 
#define CONS(a,b)  int(a##e##b) 
Printf(STR(vck));           // ����ַ���"vck" 
printf("%d\n", CONS(2,3));  // 2e3 ���:2000 

�����������һ�����ʱ�� 
��Ҫע����Ƿ��궨��������"#"��"##"�ĵط�������ǲ�����չ��. 
#define A          (2) 
#define STR(s)     #s 
#define CONS(a,b)  int(a##e##b) 
printf("%s\n", CONS(A, A));               // compile error  
��һ�����ǣ� 
printf("%s\n", int(AeA)); 
INT_MAX��A�������ٱ�չ��, Ȼ������������ķ����ܼ�. �Ӷ�һ���м�ת����. 
������������ǰ����к�Ĳ����������ȫ��չ��, ��ô��ת���������һ����(_STR)���ܵõ���ȷ�ĺ����
#define STR(s)      _STR(s)          // ת���� 
#define CONS(a,b)   _CONS(a,b)       // ת���� 
printf("int max: %s\n", STR(INT_MAX));          // INT_MAX,int�͵����ֵ,Ϊһ������ #include<climits> 
���Ϊ: int max: 0x7fffffff 
STR(INT_MAX) -->  _STR(0x7fffffff) Ȼ����ת�����ַ���; 

printf("%d\n", CONS(A, A)); 
���Ϊ��200 
CONS(A, A)  -->  _CONS((2), (2))  --> int((2)e(2)) 

"#"��"##"��һЩӦ������ 
1���ϲ����������� 
#define  ___ANONYMOUS1(type, var, line)  type  var##line 
#define  __ANONYMOUS0(type, line)  ___ANONYMOUS1(type, _anonymous, line) 
#define  ANONYMOUS(type)  __ANONYMOUS0(type, __LINE__) 
����ANONYMOUS(static int);  ��: static int _anonymous70;  70��ʾ�����к�; 
��һ�㣺ANONYMOUS(static int);  -->  __ANONYMOUS0(static int, __LINE__); 
�ڶ��㣺                        -->  ___ANONYMOUS1(static int, _anonymous, 70); 
�����㣺                        -->  static int  _anonymous70; 
��ÿ��ֻ�ܽ⿪��ǰ��ĺ�,����__LINE__�ڵڶ�����ܱ��⿪; 

2�����ṹ 
#define  FILL(a)   {a, #a} 

enum IDD{OPEN, CLOSE}; 
typedef struct MSG{ 
  IDD id; 
  const char * msg; 
}MSG; 

MSG _msg[] = {FILL(OPEN), FILL(CLOSE)}; 
�൱�ڣ� 
MSG _msg[] = {{OPEN, "OPEN"}, 
              {CLOSE, "CLOSE"}}; 

3����¼�ļ��� 
#define  _GET_FILE_NAME(f)   #f 
#define  GET_FILE_NAME(f)    _GET_FILE_NAME(f) 
static char  FILE_NAME[] = GET_FILE_NAME(__FILE__); 

4���õ�һ����ֵ��������Ӧ���ַ��������С 
#define  _TYPE_BUF_SIZE(type)  sizeof #type 
#define  TYPE_BUF_SIZE(type)   _TYPE_BUF_SIZE(type) 
char     buf[TYPE_BUF_SIZE(INT_MAX)]; 
     -->  char  buf[_TYPE_BUF_SIZE(0x7fffffff)]; 
     -->  char  buf[sizeof "0x7fffffff"]; 
�����൱�ڣ� 
char  buf[11]; 
