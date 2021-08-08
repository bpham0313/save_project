USE save_project;
DROP TABLE IF EXISTS transactions;
DROP TABLE IF EXISTS accounts;

CREATE TABLE accounts(
account_number VARCHAR(15) NOT NULL,
last_name VARCHAR(255) NOT NULL, 
first_name VARCHAR(255) NOT NULL,
street_address VARCHAR(255) NOT NULL,
unit VARCHAR(20),
city VARCHAR(255) NOT NULL,
state VARCHAR(10) NOT NULL,
zip VARCHAR(10) NOT NULL,
dob DATE NOT NULL,
ssn VARCHAR(20) NOT NULL,
email_address VARCHAR(255) NOT NULL,
mobile_number CHAR(10) NOT NULL,
PRIMARY KEY (account_number));


CREATE TABLE transactions(
account_number VARCHAR(15) NOT NULL,
transaction_number INT NOT NULL,
transaction_datetime DATETIME NOT NULL, 
transaction_amount VARCHAR(20) NOT NULL,
post_date DATE NOT NULL,
merchant_number VARCHAR(15) NOT NULL,
merchant_description VARCHAR(255) NOT NULL,
merchant_category_code VARCHAR(15) NOT NULL,
PRIMARY KEY (account_number,transaction_number),
FOREIGN KEY (account_number) REFERENCES accounts(account_number)
);


LOAD DATA INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/account_info.csv' 
INTO TABLE accounts
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS 
(last_name, first_name, street_address, unit,  city, state,  zip, @dob, ssn,  email_address, mobile_number, @account_number )
SET  account_number = TRIM(@account_number),
dob  = STR_TO_DATE(@dob, '%m/%d/%Y')
;





LOAD DATA INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/transactions.csv' 
INTO TABLE save_project.transactions
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\r\n'
IGNORE 1 ROWS 
(@account_number, @transaction_datetime,@transaction_amount,@post_date,@merchant_number,merchant_description,merchant_category_code,@transaction_number)
SET account_number =RTRIM(@account_number),
transaction_number = @transaction_number,
transaction_datetime  = STR_TO_DATE(@transaction_datetime, '%m%d%Y %H:%i:%S'),
transaction_amount = CONVERT(SubStr(@transaction_amount, 1, LENGTH(@transaction_amount) - 1),DECIMAL(15,2)),
post_date = STR_TO_DATE(@post_date, '%m%d%Y'),
merchant_number = TRIM(@merchant_number);


#Data cleaning

SET SQL_SAFE_UPDATES = 0;
UPDATE transactions SET merchant_description = trim(regexp_replace(merchant_description,'[ \t]{2,}',' '));
ALTER TABLE transactions ADD merchant VARCHAR(255) NOT NULL;
UPDATE transactions SET merchant = ( SELECT 
    CASE
        WHEN LOCATE('*', merchant_description)>0 THEN SUBSTRING_INDEX(merchant_description, '*', 1)
        WHEN LOCATE('#',merchant_description )>0 THEN SUBSTRING_INDEX(merchant_description, '#', 1)
        WHEN LOCATE('  ', merchant_description)>0 THEN SUBSTRING_INDEX(merchant_description, '  ', 1)
        ELSE merchant_description
    END );

UPDATE transactions SET merchant = trim(regexp_replace(merchant,'[0-9]+',''));
