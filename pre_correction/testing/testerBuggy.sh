echo "\n\n\033[33m=== 1. GET REQUEST ON MAIN PAGE - LACKING CR LF ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\nHost:localhost:8080\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 2. GET REQUEST ON MAIN PAGE - GET WITH BODY ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\r\nHost:localhost:8080\r\nContent-Length:3\r\n\r\nabc" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 3. PUT UPLOAD ON LOCATION SITE - BODY SIZE MISMATCH ===\033[0m\n"
printf "PUT /site/myfile.txt HTTP/1.1\r\nHost:localhost:8080\r\nContent-Length:3\r\nContent-Type:text/plain\r\n\r\nab" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 4. PUT UPLOAD ON LOCATION SITE, CHUNKED - CHUNKED PART SIZE MISMATCH ===\033[0m\n"
printf "PUT /site/myfile.txt HTTP/1.1\r\nHost:localhost:8080\r\nContent-Type:text/plain\r\nTransfer-Encoding:chunked\r\n\r\n4\r\nabcd\r\n2\r\ne\r\n0\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 5. POST UPLOAD ON LOCATION SITE, MULTIPART - PART WITH NO NAME ===\033[0m\n"
printf 'POST /site/ HTTP/1.1\r\nHost:localhost:8080\r\nContent-Type:multipart/form-data; boundary=+++\r\nContent-Length:198\r\n\r\n--+++\r\nContent-Disposition: form-data; name="file1"\r\nContent-Type: text/plain\r\n\r\nA\nB\nC\n--+++\r\nContent-Disposition: form-data; name="file2"; filename="m2.txt"\r\nContent-Type: text/plain\r\n\r\n1\n2\n3\n--+++' | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 6. PUT UPLOAD ON LOCATION SITE - BODY SIZE TOO LARGE ===\033[0m\n"
printf "PUT /site/myfile.txt HTTP/1.1\r\nHost:localhost:8080\r\nContent-Length:624\r\nContent-Type:text/plain\r\n\r\nabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 7. PUT UPLOAD ON LOCATION SECRETPLACE - PUT FORBIDDEN ===\033[0m\n"
printf "PUT /site/secretplace/myfile.txt HTTP/1.1\r\nHost:localhost:8080\r\nContent-Length:3\r\nContent-Type:text/plain\r\n\r\nab" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 8. POST UPLOAD ON LOCATION APP - UPLOAD FORBIDDEN ===\033[0m\n"
printf "POST /app/myfile.txt HTTP/1.1\r\nHost:localhost:8080\r\nContent-Length:3\r\nContent-Type:text/plain\r\n\r\nabc" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 9. GET ON MAIN PAGE - WRONG HTTP VERSION ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/2.0\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 10. GET ON MAIN PAGE - WRONG HOST HEADER ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\r\nHost 127.0.0.1\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 11. GET ON MAIN PAGE - MULTIPLE HOST HEADER ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\r\nHost: 127.0.0.1\r\nHost: example.com\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 12. GET ON MAIN PAGE - NO HOST HEADER ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 13. POST UPLOAD ON LOCATION SITE - WRONG TRANSFER-ENCODING VALUE ===\033[0m\n"
printf "POST /site/ HTTP/1.1\r\nHost: 127.0.0.1\r\nTransfer-Encoding: some\r\nContent-Type:text/plain\r\n\r\n5\r\nHello\r\n0\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 14. REQUEST ON MAIN PAGE - UNKNOWN METHOD ===\033[0m\n"
printf "FOO / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 15. GET REQUEST ON MAIN PAGE - NO HEADER END ===\033[0m\n"
printf "GET /site/welcomepage.html HTTP/1.1\r\nHost: 127.0.0.1\r\nHello" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 16. POST UPLOAD ON LOCATION SITE - TRANSFER-ENCODING CHUNKED WITH CONTENT-LENGTH ===\033[0m\n"
printf "POST /site/ HTTP/1.1\r\nHost: 127.0.0.1\r\nContent-Type:text/plain\r\nContent-Length: 11\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nHello\r\n0\r\n\r\n" | nc -N 127.0.0.1 8080

echo "\n\n\033[33m=== 17. GET REQUEST ON MAIN PAGE - MANGLED FIRST LINE ===\033[0m\n"
printf "GET /HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n" | nc -N 127.0.0.1 8080