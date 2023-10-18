# Secure_Address_Book (with AES, Base64, MySQL)  

<img src="https://github.com/kafkaaaa/Secure_Address_Book/assets/20926959/dde13b80-0f2d-4a40-9a49-06ba98bf046b">
 
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
<img src="https://github.com/kafkaaaa/Secure_Address_Book/assets/20926959/b7e41013-e49b-497c-be86-86e805d2c3ff">

### DBeaver
<img src="https://github.com/kafkaaaa/Secure_Address_Book/assets/20926959/0d7b85ff-84f3-4ca1-baf1-17ad1ceb8e21">
  
## Ref
- <https://github.com/kokke/tiny-AES-c>  
- <https://gist.github.com/erev0s/4c13a5173b607bcea2a1f273981ccdfc>
