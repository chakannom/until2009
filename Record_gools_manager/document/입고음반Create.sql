CREATE TABLE �԰�����(
�԰��ȣ CHAR(10) NOT NULL PRIMARY KEY,
�԰����� DATE NOT NULL,
�԰�ܰ� INT NOT NULL,
�԰���� INT NOT NULL,
�����ڵ� CHAR(10) NOT NULL,
CONSTRAINT �����_�ȵ�_�����ڵ��� FOREIGN KEY(�����ڵ�) REFERENCES ����(�����ڵ�)
);
