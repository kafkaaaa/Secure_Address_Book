# 암/복호화(AES) 기능이 있는 주소록 
  
-CLI(command line interface)에서 실행되도록 연결리스트로 구현  
-기능 -> 추가, 삭제, 출력이 되도록 가능하면 업데이트도 (CRUD)  
-프로그램 종료할때 리스트에 있는 Data는 File로 저장  
-프로그램 실행할때 파일에 저장되어 있는거 자동 불러오기  
  


-주소록 구조에 ID 개념 추가  
-사용자 입력값 검사(제한) 추가  
-AES 알고리즘 적용(AES-256, MODE= CBC, Padding= PKCS7) 
-일반 텍스트가 아닌 바이너리 파일로 저장  
-공백 문제 해결  
-헤더 파일 분리  
-Makefile 작성  
-동적 라이브러리(shared lib) 구성  
  

# Build  
$ make clean  
$ make  
$ ./addr_book  
$ output.bin  