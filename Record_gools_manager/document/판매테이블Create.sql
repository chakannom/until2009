CREATE TABLE �Ǹ� (
��ID CHAR(10) NOT NULL,
�����ڵ� CHAR(10) NOT NULL,
�Ǹ��� DATE NOT NULL,
���� INT NULL,
CONSTRAINT fk�Ǹ�tbl_��ID��_���� FOREIGN KEY(��ID) REFERENCES ��(��ID),
CONSTRAINT fk�Ǹ�tbl_�����ڵ尡_���� FOREIGN KEY(�����ڵ�) REFERENCES ����(�����ڵ�)
);
