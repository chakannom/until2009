고객등록
INSERT INTO 고객 VALUES('test_c','나호정','123-1113','123456-1111113','성황동',0);

음반입고(음반목록에 없을 경우)
INSERT INTO 음반 VALUES('1','SCANDAL 1집','강타와 바네스','SM엔터테이먼트',10,8900);
INSERT INTO 입고음반 VALUES('1','06/5/20',7000,10,'1');

음반입고(음반목록에 없을 있을 경우)
INSERT INTO 입고음반 VALUES('9','06/5/25',7000,10,'1');

고객정보 전제보기(고객ID로 정렬)
SELECT * FROM 고객 ORDER BY 고객ID;

음반목록 전제보기(음반코드로 정렬)
SELECT * FROM 음반 ORDER BY 음반코드;

판매목록 전제보기
SELECT * FROM 판매;

입고목록 전제보기
SELECT * FROM 입고음반;

음반검색(음반코드로)
SELECT * FROM 음반 WHERE 음반코드='1';

음반검색(음반이름으로)
SELECT * FROM 음반 WHERE 음반이름 like '%눈%';

음반검색(가수로)
SELECT * FROM 음반 WHERE 가수 like '%이%';

음반검색(음반이름과 가수로)
SELECT * FROM 음반 WHERE 가수 like '%이%' and 음반이름 like '%눈%';

고객검색(고객ID로)
SELECT * FROM 고객 WHERE 고객ID='test_a';

고객검색(전화번호로)
SELECT * FROM 고객 WHERE 전화번호='123-1111';

재고수량수정(고객이 물건을 샀을 경우)프로그램에서 값을 빼고 UPDATE함
UPDATE 음반 SET 재고수량=10 WHERE 음반코드='1';