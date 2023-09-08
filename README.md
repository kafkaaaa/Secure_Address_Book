# Address Book with AES Enc/Decryption
-Data Structure: Linked List  
-CRUD (추가 수정 삭제 조회)
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
  
  
# Ref
-https://github.com/kokke/tiny-AES-c  
-https://gist.github.com/erev0s/4c13a5173b607bcea2a1f273981ccdfc
