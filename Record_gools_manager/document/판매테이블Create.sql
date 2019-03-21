CREATE TABLE 판매 (
고객ID CHAR(10) NOT NULL,
음반코드 CHAR(10) NOT NULL,
판매일 DATE NOT NULL,
수량 INT NULL,
CONSTRAINT fk판매tbl_고객ID가_없음 FOREIGN KEY(고객ID) REFERENCES 고객(고객ID),
CONSTRAINT fk판매tbl_음반코드가_없음 FOREIGN KEY(음반코드) REFERENCES 음반(음반코드)
);
