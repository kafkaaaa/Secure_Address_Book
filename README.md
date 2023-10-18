# Secure_Address_Book (with AES, Base64, MySQL)  

<img src="https://github.com/kafkaaaa/chat/assets/20926959/17a033a1-03db-4857-991e-c67df5d37b99">
 
 - Data Structure: Linked List  
 - CRUD (Insert Update Delete Select)
 - **AES Encrpytion (AES-256, MODE= CBC, Padding= PKCS7)**
 - When program terminated, data in the linked-list is stored as a file.
 - When program started, data in the file is loaded.  
 - Distinguish each person by ID
 - Not normal text, save to **Binary Data**    
 - Makefile  
 - Shared Library  

## **addr_book_db**
- private data(phone number) : Base64 Encoded
- Add a menu to view the private data when authenticated by DB password
- Plain Text -> (AES-256/CBC) Encryption -> Binary Data -> (Base64) Encoding
- (Base64) Encoded Text -> Decoding (Base64) -> Binary Data -> Decryption (AES-256/CBC) -> Plain Text
  
  
## Build
```
$ make clean  
$ make  
$ ./addr_book  
$ output.bin    (*example)
```
  

## Example (addr_book_db)

### ./addr_book
<img src="https://github.com/kafkaaaa/Secure_Address_Book/assets/20926959/0b4bfc4f-b851-4ff5-b559-caaa746a6945">

### DBeaver
<img src="https://github.com/kafkaaaa/Secure_Address_Book/assets/20926959/991dcebb-fda3-4bd2-a2bc-9a74b30c96bf">
  
## Ref
- <https://github.com/kokke/tiny-AES-c>  
- <https://gist.github.com/erev0s/4c13a5173b607bcea2a1f273981ccdfc>
