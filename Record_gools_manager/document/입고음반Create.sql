CREATE TABLE 입고음반(
입고번호 CHAR(10) NOT NULL PRIMARY KEY,
입고일자 DATE NOT NULL,
입고단가 INT NOT NULL,
입고수량 INT NOT NULL,
음반코드 CHAR(10) NOT NULL,
CONSTRAINT 등록이_안된_음반코드임 FOREIGN KEY(음반코드) REFERENCES 음반(음반코드)
);
