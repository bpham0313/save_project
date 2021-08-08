# save_project
C++ program to detect fraud transactions 

*Create data for the program :

Run ./MySQL/create_data.sql (change the LOAD DATA INFILE command directory) or ./MySQL/save_project.sql to create save_project database

*Run the program :

The program can be executed by running the .exe file ./x64/Release/ 

The program will ask for hostname (localhost:xxxx), username, and password to access the database. 

*Results:
Hostname: localhost:3306
User: root

Password
======================================== Rule 1 : Identify Abnormally High Transactions==========================
Name                     Account Number           Transaciton Number       Merchant                           Transaction Amount
Michael Smith            11111                    15                       KROGER                             $2300.00
Albares Cammy            14829                    24                       SQ                                 $295.27
Vocelka Francine         20759                    58                       TST                                $387.34
John Doe                 22222                    10                       Best Buy New York NY               $10000.00
Tollner Mitsue           25643                    57                       JASON'S DELI PTX  PEARLAND IL      $20.73
Whobrey Yuki             26607                    30                       PEET'S                             $5.45
Luczki Yolando           26995                    10                       Wal-Mart Super Center CORINTH NY   $1.80
Pugh Blondell            30808                    2                        EFT Santa Fe RI                    $393.25
Monica Abel              33555                    10                       Best Buy New York NY               $10000.00
Ruth Green               37222                    1                        SHELL OIL  CA                      $1009.90
Morasca Simona           38829                    164                      CHEVRON/HANDI PLUS                 $3.50
Artie Venn               44891                    10                       Best Buy New York NY               $10000.00
Lenord Paps              46762                    3                        Burger King AK                     $85.04
Mirasca Sam              53222                    10                       Kohls Ashland OH                   $725.51
Tanner Mitchel           54566                    4                        Starbucks Chicago IL               $100.73
Davis Cris               54992                    1                        Taco Bell  MD                      $100.90





======================================== Rule 2 : Identify Out Of State Transactions==========================
Name                     Account Number           Transaciton Number       Expected Transaction Location           Actual Transaction Location
Michael Smith            11111                    25                       Texas                                   Florida
John Doe                 22222                    10                       California                              New York
Jane Doe                 33333                    2                        New York                                New Jersey
Monica Abel              33555                    13                       New York                                California
Ruth Green               37222                    1                        Ohio                                    California
Ruth Green               37222                    2                        Ohio                                    California
Ruth Green               37222                    3                        Ohio                                    California
Ruth Green               37222                    4                        Ohio                                    California
Ruth Green               37222                    5                        Ohio                                    California
Ruth Green               37222                    6                        Ohio                                    Georgia
Joseph Dark              43991                    16                       Michigan                                New York
Artie Venn               44891                    10                       New Jersey                              New York
Don Fuller               53111                    16                       Ohio                                    New York
Anthony West             54891                    7                        South Dakota                            Maryland
Anthony West             54891                    10                       South Dakota                            New York
Anthony West             54891                    13                       South Dakota                            California

C:\Users\p\source\repos\SAVE_Project\x64\Release\SAVE_Project.exe (process 31084) exited with code 0.
Press any key to close this window . . .
