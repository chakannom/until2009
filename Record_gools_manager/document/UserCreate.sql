CREATE USER prodb
IDENTIFIED BY prodb
DEFAULT TABLESPACE prodb
TEMPORARY TABLESPACE temp
QUOTA UNLIMITED on prodb
QUOTA UNLIMITED on temp;

grant connect, resource to prodb;