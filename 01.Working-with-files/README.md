## Работа с файлове

**Зад 1. 02-а-0100** <br />
​Направете копие на файла **/etc/passwd** във вашата home директория под името **my_passwd**.

​	`cp /etc/passwd ~/my_passwd` <br />

**Зад 2. 02-а-0600** <br />
​Направете директория **practice-test** в home директорията ви. 
​Вътре направете директория **test1**. Можете ли да направите тези две неща наведнъж? Разгледайте нужната man страница.
​След това създайте празен файл вътре, който да се казва **test.txt**, преместете го в practice-test чрез релативни пътища.

​	`mkdir -p practice_test/test1` <br />
​	`touch test.txt` <br />
​	`mv test.txt practice_test` <br />

**Зад 3. 02-a-0600** <br />
​Копирайте файловете **f1, f2, f3** от директорията **/tmp/os2018/practice/01/** в директория **dir1**, намираща се във вашата home директория. Ако нямате такава, създайте я.

​	`mkdir -p  /tmp/os2018/practice/01` <br />
​	`cd /tmp/os2018/practice/01` <br />
​	`touch f1 f2 f3` <br />
​	`mkdir dir1` <br />
​	`cp /tmp/os2018/practice/01/{f1,f2,f3} ~/dir1`

**Зад 4. 02-a-0601** <br />
​Нека файлът **f2** бъде преместен в директория **dir2**, намираща се във вашата **home** директория и бъде преименуван на **numbers**.

​	`mkdir ~/dir2 && cp ~/dir1/f2 ~/dir2/numbers`

**Зад 5. 02-a-1200** <br />
​Отпечатайте имената на **всички директории** в директорията **/home**.

​	`find /home -type d`

**Зад 6. 02-a-2100** <br />Създайте **symlink** на файла **/etc/passwd** в home директорията ви (да се казва например passwd_symlink).l

​	`ln -s /etc/passwd ~/passwd_symlink` 

**Зад 7. 02-а-4000** <br />Създайте файл permissions.txt в home директорията си. За него дайте единствено - read права на потребителя създал файла, write and exec на групата, read and exec на всички останали. Направете го и с битове, и чрез "буквички".

​	`touch permissions.txt`
​	`chmod u=r,g=wx,o=rx permissions.txt`
​	`chmod 435 permissions.txt` 

**Зад 8. 02-а-4100**<br />За да намерите какво сте правили днес: намерете всички файлове в home директорията ви, които са променени от вас в последния 1 час.

​	`find ~ -type f -mmin -60`

**Зад 9. 02-а-5000**<br />Копирайте secret.txt от **/tmp/os2018/02/** в home директорията си. Прочетете го с командата cat.

​	`cp /tmp/os2018/os/02/secret.txt ~/`
​	`cat secret.txt`

**Зад 10. 02-a-5400** <br />Изведете всички обикновени ("regular") файлове, които **/etc** и нейните преки поддиректории съдържат

​	`find /etc -maxdepth 1 -type f`

**Зад 11. 02-a-5401**<br />Създайте файл, който да съдържа само първите 5 реда от изхода на 8)

​	`find /etc -maxdepth 1 -type f | head -n 5 > result.txt`

**Зад 12. 02-а-5402**<br/>Изведете всички обикновени ("regular") файлове, които само преките поддиректории на **/etc** съдържат

​	`find /etc -mindepth 2 -maxdepth 2 -type f`

**Зад 13. 02-а-5403** <br />
Изведете всички преки поддиректории на /etc

​	`find /etc -maxdepth 1 -type d`

**Зад 14. 02-a-5500** <br />Създайте файл, който да съдържа само последните 10 реда от изхода на 11)

​	`find /etc -maxdepth 1 -type d | tail -n 10 > result.txt`

**Зад 15. 02-а-5501** <br />Изведете обикновените файлове по-големи от 42 байта в home директорията ви

​	`find ~ -maxdepth 1 -type f -size +42c`	

**Зад 16. 02-а-5503**<br />Изведете всички обикновени файлове в директорията SI които са от групата students

​	`find /home/SI -type f -group "student" 2>/dev/null`

**Зад 17. 02-a-5504** <br/>Изведете всички обикновени файлове в директорията SI които са от групата students, които имат write права за достъп за група или за останалите(o=w) // ако в свободното си време искате да пишете по файлове на други хора

​	`find . -perm -g-w -or -perm -o-w`

**Зад 18. 02-a-5505** <br />Изведете всички файлове, които са по-нови от създадения файл в 9)

​	`find . -type f -newer "result.txt"`

**Зад 19. 02-a-5506** <br />Изтрийте файловете по-нови от създаденият в 9) файл

​	`find . -type f -newer "result.txt" -exec rm -i {} \;`

**Зад 20. 02-a-6000** <br/>Намерете файловете в /bin, които могат да се четат, пишат и изпълняват от всички.

​	`find /bin -type f -perm 777`

**Зад 21. 02-а-8000** <br />Копирайте всички файлове от /bin, които могат да се четат, пишат и изпълняват от всички, в bin2 директория в home директорията ви. Направете такава, ако нямате.

​	`find /bin -type f -perm 777 -exec cp '{}' ~/bin2 \;`

**Зад 22. 02-а-9000** <br />Oт предната задача: когато вече сте получили home/../../bin2 с команди, архивирайте всички команди вътре, които започват с b в архив, който се казва b_start.tar. (командата, която архивира е tar -c -f <файл1> <файл2>...)	

​	`tar -c -f b_start.tar b*`

**Зад 23. 02-а-9500** <br />Използвайки едно извикване на командата find, отпечатайте броя на редовете във всеки обикновен файл в /home директорията.

​	`find . -maxdepth 1 -type f -name "*.txt" | xargs -I 'var' bash -c 'echo "File var has $(cat "var" | wc -l) lines"';`

**Зад 24. 02-b-4000** <br/>Копирайте най-големия файл от тези, намиращи се в /tmp/os2018/02/bytes/, в home директорията си.

​	`du /tmp/os2018/02/bytes/ | sort -n -r | head -n 1 | cut -f2 | xargs -I{} cp "{}" ~/`