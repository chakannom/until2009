�����
INSERT INTO �� VALUES('test_c','��ȣ��','123-1113','123456-1111113','��Ȳ��',0);

�����԰�(���ݸ�Ͽ� ���� ���)
INSERT INTO ���� VALUES('1','SCANDAL 1��','��Ÿ�� �ٳ׽�','SM�������̸�Ʈ',10,8900);
INSERT INTO �԰����� VALUES('1','06/5/20',7000,10,'1');

�����԰�(���ݸ�Ͽ� ���� ���� ���)
INSERT INTO �԰����� VALUES('9','06/5/25',7000,10,'1');

������ ��������(��ID�� ����)
SELECT * FROM �� ORDER BY ��ID;

���ݸ�� ��������(�����ڵ�� ����)
SELECT * FROM ���� ORDER BY �����ڵ�;

�ǸŸ�� ��������
SELECT * FROM �Ǹ�;

�԰��� ��������
SELECT * FROM �԰�����;

���ݰ˻�(�����ڵ��)
SELECT * FROM ���� WHERE �����ڵ�='1';

���ݰ˻�(�����̸�����)
SELECT * FROM ���� WHERE �����̸� like '%��%';

���ݰ˻�(������)
SELECT * FROM ���� WHERE ���� like '%��%';

���ݰ˻�(�����̸��� ������)
SELECT * FROM ���� WHERE ���� like '%��%' and �����̸� like '%��%';

���˻�(��ID��)
SELECT * FROM �� WHERE ��ID='test_a';

���˻�(��ȭ��ȣ��)
SELECT * FROM �� WHERE ��ȭ��ȣ='123-1111';

����������(���� ������ ���� ���)���α׷����� ���� ���� UPDATE��
UPDATE ���� SET ������=10 WHERE �����ڵ�='1';