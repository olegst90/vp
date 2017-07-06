msg: .STR "Hello,\sworld"
buff: .HEX xx"deadbeaf"
.BYTE 10
.BYTE 255
;currently, floats are not supported
;.WORD 3.00
.GLOBAL @start

start:
   LDA
