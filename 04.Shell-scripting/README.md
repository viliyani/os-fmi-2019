## Shell скриптове

**Зад 1. 05-а-2000**<br/>Сменете вашия prompt с нещо по желание. После върнете оригиналния обратно.

`PS_OLD=$PS1`

`$PS1="enter command"`

`PS1=$PS_Old`

**Зад 2. 05-а-2100**<br/>Редактирайте вашия .bash_profile файл, за да ви поздравява (или да изпълнява някаква команда по ваш избор) всеки път, когато влезете в системата.

`vi ~/.bash_profile`  insert `echo "Welcome to unix, dear friend!"`

**Зад 3. 05-а-2200**<br/>Направете си ваш псевдоним (alias) на полезна команда.

`vi ~/bashrc` insert `alias c='clear'`

**Зад 4. 05-а-4000**<br/>Напишете shell script (който ще използвате от вашия линукс), който да приема параметър име на файл, който искате да копирате от os-server и да го копира в текущата директория (на вашия линукс).

Пример за употреба:

```
# сваля /etc/passwd от os-server на вашия линукс
./get_remote_file /etc/passwd
```

`vi ~/bin/get_remote_file`

```bash
#!/bin/bash

# get_remote_file
# This script copies a file from remote unix to local unix machine

# Check for an argument
if [[ ! $1  ]]; then
    echo "Missing filename argument"
    exit 1
fi

filename="$1"
destination="$PWD"

echo "Trying to get ${filename} from remote server"

scp "s62219@os-server:${filename}" "${destination}"
```

`chmod +x ~/bin/get_remote_file`

**Зад 5. 05-b-2000**<br/>Да се напише shell скрипт, който приканва потребителя да въведе име (низ) и изпечатва "Hello, името".

````bash
#!/bin/bash

echo "Enter name: ";
read name;
echo "Hello,  " "${name}";
````

**Зад 6. 05-b-2800**<br/>Да се напише shell скрипт, който проверява дали подаденият му параметър се състои само от букви и цифри.

````bash
#!/bin/bash

if [[ $1 =~ [a-zA-Z0-9]+ ]]; then
  echo "Only alpha and digit";
fi;
````

**Зад 7. 05-b-3000**<br/>Да се напише shell скрипт, който извежда на стандартния изход факултетния номер на потребителя, който изпълнява скрипта, или казва "DANGER! It's a teacher!", ако потребителското име не е факултетен номер.

``````bash
#!/bin/bash

user=$(id -un)

if [[ "${user}" =~ s62219 ]]; then
  echo "${user}" | cut -c 2-
else
  echo "DANGER...";
fi
``````

**Зад 8. 05-b-3100**<br/>Да се напише shell скрипт, който приканва потребителя да въведе низ - потребителско име на потребител от системата - след което извежда на стандартния изход колко пъти потребителят се е логнал.

``````bash
#!/bin/bash

echo "Enter username";

read user;

total_logins=$(last | grep "${user}" -o | wc -l)

echo "Total logins for $user:   $total_logins";
``````

**Зад 9. 05-b-3200**<br/>Да се напише shell скрипт, който приканва потребителя да въведе пълното име на директория и извежда на стандартния изход подходящо съобщение за броя на всички файлове и директории в нея.

````bash
#!/bin/bash

echo "Enter full path to dir: ";

read dname;

total=$(find $dname 2> /dev/null | wc -l);

echo "Total files: $total";
````

**Зад 10. 05-b-3300**<br/>Да се напише shell скрипт, който чете от стандартния вход имената на 3 файла, обединява редовете на първите два, подрежда ги по азбучен ред и резултата записва в третия файл.

```  bash
#!/bin/bash

read file1
read file2
read file3

cat "${file1}" "${file2}" | sort > "${file3}"  
```

**Зад 11. 05-b-3400**<br/>Да се напише shell скрипт, който чете от стандартния вход име на файл и символен низ, проверява дали низа се съдържа във файла и извежда на стандартния изход кода на завършване на командата.

```bash
#!/bin/bash

read file;
read str;

if grep -q "${str}" "${file}"; then
  echo "Found";
else
  echo "Not found";
fi
```

**Зад 12. 05-b-4200** <br/>Имате компилируем (a.k.a няма синтактични грешки) source file на езика C. Напишете shell script, който да покзва колко е дълбоко най-дълбокото nest-ване (влагане). По-лесно ли щеше да е за файл с Python код?

`````bash
#!/bin/bash

file=$1
currentCNT=0
maxCNT=0


while read LINE; do
  if [ "${LINE}" = "{" ]; then
    (( currentCNT++ ))
    
    if [ $currentCNT -gt $maxCNT ]; then
      maxCNT=$currentCNT;
    fi
  else  
    (( currentCNT-- ))
  fi

done < <(cat $1 | egrep -o '[\{\}]')

if [[ $currentCNT -eq 0 ]]; then
  echo "Good balance";
else
  echo "Bad balance";
fi

echo "The deepest nesting is $maxCNT levels"
`````

**Зад 13. 05-b-4300**<br/>Напишете shell script, който по подаден като аргумент файл с map между \<nickname\> -> <реален sername в os-server> и nickname, ще ви улесни да изпращате съобщения на хората.

Пример за файл указател: 

```
tinko	s61966
minko	s881234
ginko	s62000
dinko	s77777
senpai	velin
```

Примерно извикване на програмата:<br/>./send_message myAddressBook dinko

`````bash
#!/bin/bash

address_book=$1
target=$2

if [[ ! -r $address_book ]]; then
    echo "Error! ${address_book} is not readable"
    exit 1
fi

if [[ $(awk '{print NF}' $address_book | sort -nu) -ne "2" ]]; then
    echo "Address book must have exactly 2 columns"
    exit 1
fi

target_username=$(cat "${address_book}" | egrep "$target " | cut -d ' ' -f 2)
if [[ -z $target_username ]]; then
    echo "Username with nickname ${target} cannot be found in ${address_book}"
    exit 1
fi

read -p "Enter message: " message
echo "Sending message to ${target_username} "
write ${target_username} "${message}"
exit # Exit with exit code from last command
`````

**Зад 14. 05-b-4301** <br/>Напишете shell script, който автоматично да попълва файла указател от предната задача по подадени аргументи: име на файла указател, пълно име на човека (това, което очакваме да е в /etc/passwd) и избран за него nickname.<br/>Файлът указател нека да е във формат:<br/><nickname, който лесно да запомните> <username в os-server>

Примерно извикване:<br/>./pupulate_address_book myAddressBook "Ben Dover" uncleBen

Добавя към myAddressBook entry-то:<br/>uncleBen <username на Ben Dover в os-server>

*Бонус: Ако има няколко съвпадения за въведеното име (напр. има 10 човека Ivan Petrov в /etc/passwd), всички те да се показват на потребителя, заедно с пореден номер >=1,след което той да може да въведе някой от номерата (или 0 ако не си хареса никого), и само избраният да бъде добавен към указателя.*

`````` bash
#!/bin/bash

file=$1
name="${2}"
nickname=$3

username=$(cat /etc/passwd | grep "${name}" | cut -d: -f1)

echo "${nickname} ${username}" >> "${file}";
``````

**Зад 15. 05-b-4400**<br/>Напишете shell script, който да приема параметър име на директория, от която взимаме файлове, и опционално експлицитно име на директория, в която ще копираме файлове. Скриптът да копира файловете със съдържание, променено преди по-малко от 45 мин, от първата директория във втората директория. Ако втората директория не е подадена по име, нека да получи такова от днешната дата във формат, който ви е удобен. При желание новосъздадената директория да се архивира.

``````bash
#!/bin/bash

if [ -n "${2}" ]; then
  name="$2"
else
  name=$(date)
fi

find "${1}" -maxdepth 2 -type f -mmin +45 2>/dev/null | head -n 10 |
 xargs tar cfv "${name}.tar" 
``````

**Зад 16. 05-b-4500**<br/>Да се напише shell скрипт, който получава при стартиране като параметър в командния ред идентификатор на потребител. Скриптът периодично (sleep(1)) да проверява дали потребителят е log-нат, и ако да - да прекратява изпълнението си, извеждайки на стандартния изход подходящо съобщение.

`````` bash
#!/bin/bash

read -p "Enter username: " username

#who | awk '{print $1}';
#who | awk '{print $1}' | egrep -q "^${username}$" --color;
#echo $?;

while [[ $(who | awk '{print $1}' | egrep -q "^${username}$" && echo $? || echo $? ) -eq 1 ]]; do
  sleep 1;
  echo "-> Waiting $username to log in...";
done;

echo "--> Success! $username is logged!";
``````

**Зад 17. 05-b-4600**<br/>Да се напише shell скрипт, който валидира дали дадено цяло число попада в целочислен интервал. Скриптът приема 3 аргумента: числото, което трябва да се провери; лява граница на интервала; дясна граница на интервала.

Скриптът да връща exit status:

- 3, когато поне един от трите аргумента не е цяло число

- 2, когато границите на интервала са обърнати

- 1, когато числото не попада в интервала

- 0, когато числото попада в интервала

  ```
  Примери:
  $ ./validint.sh -42 0 102; echo $?
  1
  
  $ ./validint.sh 88 94 280; echo $?
  1
  
  $ ./validint.sh 32 42 0; echo $?
  2
  
  $ ./validint.sh asdf - 280; echo $?
  3
  ```

``````bash
#!/bin/bash

number="${1}";
left="${2}";
right="${3}";

if [[ "$number" =~ ^[^0-9]+$ || "$left" =~ ^[^0-9]+$ || "$right" =~ ^[^0-9]+$  ]]; then
  exit 3;
fi;

if [[ "$left" -gt "$right" ]]; then
  exit 2;
fi;

if [[ ! ("$number" -lt "$right" && "$number" -gt "$left") ]]; then
  exit 1;
fi;

if [[ "$number" -lt "$right" && "$number" -gt "$left" ]]; then
        exit 0;
fi;

exit 11;
``````

**Зад 18. 05-b-4700**<br/>Да се напише shell скрипт, който форматира големи числа, за да са по-лесни за четене. <br/>Като пръв аргумент на скрипта се подава цяло число.<br/>Като втори незадължителен аргумент се подава разделител. По подразбиране цифрите се разделят с празен интервал.

Примери:

`````
$ ./nicenumber.sh 1889734853
1 889 734 853

$ ./nicenumber.sh 7632223 ,
7,632,223
`````

``````bash
#!/bin/bash

number="${1}";
delimiter="${2}";
sign="@";

if [[ ! "$number" =~ ^[0-9]+$ ]]; then
  echo "You entered invalid number!";
  exit 1;
fi

len=$(echo "${number}" | grep . -o | wc -l);
remainder=$(( $len % 3 ));

if [[ $remainder -eq 0 ]]; then
  totalZerosToAdd=0;
else
  totalZerosToAdd=$(( 3 - $remainder ));
fi

i=0;

while [[ $i -lt $totalZerosToAdd ]]; do
  i=$(( $i + 1 ));
  number=$(echo "${sign}${number}");
done;

echo $number | grep '...' --color -o | tr "\n" "${delimiter}" | head -c -1 | tr -d '@'
echo -e "\n";
``````

**Зад 19. 05-b-4800**<br/>

Да се напише shell скрипт, който приема файл и директория. Скриптът проверява в подадената директория и нейните под-директории дали съществува копие на подадения файл и отпечатва имената на намерените копия, ако съществуват такива.

``````bash
#!/bin/bash

while IFS=$',' read -d ',' myvar; do
  echo $myvar;
done;

exit 3;

file="${1}";
dir="${2}";

if [ -z "$file" ] || [ -z "$dir" ]; then
  echo "enter file and dir, please!";
  exit 1;
fi;

if [ ! -e "$file" ]; then
  echo "you entered unexisting file!";
  exit 1;
fi;

if [ ! -d "$dir" ]; then
  echo "non existing directory!";
  exit 1;
fi;

while read line; do
  if [ -z "$(cmp "$file" "$line")" ] && [ "$(find "$1" -printf "%i\n")" != "$(find "$line" -printf "%i\n")" ]; then
    echo $line;
  fi
done < <(find "$dir" -type f);
``````

**Зад 20. 05-b-5500**<br/>

Да се напише shell script, който генерира HTML таблица със студентите в os-server. Таблицата трябва да има:

- колони за факултетен номер, име, фамилия, специалност, курс и група

- заглавен ред с имената нa колоните
  Пример:

  ```
  $ ./passwd-to-html.sh > table.html
  $ cat table.html
  <table>
    <tr>
      <th>FN</th>
      <th>Firstname</th>
      <th>Lastname</th>
      <th>Specialty</th>
      <th>Course</th>
      <th>Group</th>
    </tr>
    <tr>
      <td>60309</td>
      <td>Dragan</td>
      <td>Petkanov</td>
      <td>SI</td>
      <td>4</td>
      <td>3</td>
    </tr>
    <tr>
      <td>81111</td>
      <td>Pencho</td>
      <td>Ivanov</td>
      <td>KN</td>
      <td>3</td>
      <td>8</td>
    </tr>
  </table>
  ```

``````bash
#!/bin/bash

echo -e "<table>";
echo -e "\t<tr>"
echo -e "\t\t<th>FN</th>"
echo -e "\t\t<th>Firstname</th>"
echo -e "\t\t<th>Lastname</th>"
echo -e "\t\t<th>Speciality</th>"
echo -e "\t\t<th>Course</th>"
echo -e "\t\t<th>Group</th>"
echo -e "\t</tr>"

while read LINE; do
  fn=$(echo "${LINE}" | egrep -o "^s[0-9]+" | egrep -o "[0-9]+");
  firstname=$(echo "${LINE}" | cut -d ':' -f 5 | cut -d ',' -f 1 | cut -d ' ' -f 1);
  lastname=$(echo "${LINE}" | cut -d ':' -f 5 | cut -d ',' -f 1 | cut -d ' ' -f 2);
  speciality=$(echo "${LINE}" | cut -d ':' -f 5 | cut -d ',' -f 2 | tr -d ' ');
  course=$(echo "${LINE}" | cut -d ':' -f 5 | cut -d ',' -f 3 | tr -d ' ');
  group=$(echo "${LINE}" | cut -d ':' -f 5 | cut -d ',' -f 4 | tr -d ' ');

  echo -e "\t<tr>"
  echo -e "\t\t<th>$fn</th>"
  echo -e "\t\t<th>$firstname</th>"
  echo -e "\t\t<th>$lastname</th>"
  echo -e "\t\t<th>$speciality</th>"
  echo -e "\t\t<th>$course</th>"
  echo -e "\t\t<th>$group</th>"
  echo -e "\t</tr>"
done < <(cat passwd)

echo -e "</table>";
``````

**Зад 21. 05-b-6600**<br/>Да се напише shell скрипт, който получава единствен аргумент директория и изтрива всички повтарящи се (по съдържание) файлове в дадената директория. Когато има няколко еднакви файла, да се остави само този, чието име е лексикографски преди имената на останалите дублирани файлове.

``````bash
#!/bin/bash

if [ ! -d "${1}" ]; then
  echo "Incorrect directory!";
  exit 1;
fi

find "${1}" -type f 2>/dev/null| while read FILE1; do
  find "${1}" -type f 2>/dev/null | while read FILE2; do
    if [ -f "${FILE1}" ] && [ -f "${FILE2}" ]; then
      hash1=$(md5sum "${FILE1}" | awk '{print $1}');
      hash2=$(md5sum "${FILE2}" | awk '{print $1}');

      if [ "${hash1}" = "${hash2}" ] && [ "${FILE1}" != "${FILE2}" ]; then
        if [ "${FILE1}" \> "${FILE2}" ]; then
          rm "${FILE1}";
          # echo delete file "${FILE1}";
        fi;
      fi;
    fi;
  done;
done;
``````

**Зад 22. 05-b-7000**<br/>Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове. Скриптът да прочита от стандартния вход символен низ и за всеки от зададените файлове извежда по подходящ начин на стандартния изход броя на редовете, които съдържат низа.

```` bash
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Enter arguments!";
  exit 1;
fi;

while read -d ' ' FILE; do
   if [ ! -f "${FILE}" ]; then
      echo "Incorrect file: ${FILE}";
      exit 1;
   fi;
done < <(echo $@" ")

read -p "Enter string: " userstr;

while [ -n "${1}" ]; do
  result=$(cat "${1}" | egrep -c "${userstr}");
  echo "${1} has ${result} times string '${userstr}'";
  shift;
done;
````

**Зад 23. 05-b-7100**<br/>Да се напише shell скрипт, който приема два параметъра - име на директория и число. Скриптът да извежда на стандартния изход имената на всички обикновени файлове във директорията, които имат размер, по-голям от подаденото число.

````bash
#!/bin/bash

if [ ! -d "${1}" ]; then
  echo "Invalid directory!";
  exit 1;
fi;

if [[ ! "${2}" =~ ^[0-9]+$ ]]; then
  echo "Invalid number!";
  exit 1;
fi;

while read FILE; do
  fsize=$(stat "${FILE}" --printf='%s');
  if [ "${fsize}" -gt "${2}" ]; then
    echo "${FILE}";
  fi
done < <(find "${1}" -type f 2>/dev/null);
````

**Зад 24. 05-b-7200**<br/>
Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове или директории. Скриптът да извежда за всеки аргумент подходящо съобщение:
  = дали е файл, който може да прочетем
  = ако е директория - имената на файловете в нея, които имат размер, по-малък от броя на файловете в директорията.

``````bash
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "No arguments!";
  exit 1;
fi;

while [ -n "${1}" ]; do

  if [ -f "${1}" ]; then
    if [ -r "${1}" ]; then
      echo "--> File ${1} is readable.";
    else
      echo "--> File ${1} is not readable.";
    fi
  elif [ -d "${1}" ]; then
    entries=$(find "${1}" -type f 2>/dev/null | wc -l);
    echo "Total entries in ${1} are ${entries}";
    while read FILE; do
      fsize=$(stat "${FILE}" --printf='%s');
      if [ "${fsize}" -lt "${entries}" ]; then
        echo "+++ --> File ${FILE} in directory ${1} with size ${fsize}";
      fi
    done < <(find "${1}" -type f 2>/dev/null);
  fi

  shift;
done < <(echo "${@}");
``````

**Зад 25. 05-b-7500**<br/>Напишете shell script guess, която си намисля число, което вие трябва да познате. В зависимост от вашия отговор, програмата трябва да ви казва "надолу" или "нагоре", докато не познате числото. Когато го познаете, програмата да ви казва с колко опита сте успели.

``````bash
#!/bin/bash

randnum=$(( (RANDOM % 100) + 1  ));

guessed=0; 
tries=0;

while [ "${guessed}" -eq 0 ]; do
  read -p "Guess? " gnum;

  if [[ ! "${gnum}" =~ ^[0-9]+$ ]]; then
    echo "GAME OVER! You didn't enter a number! Sorry!";
    exit 5;   
  fi

  tries=$(( $tries + 1 ));
  if [ "${gnum}" -gt "${randnum}" ]; then
    echo "...smaller!";
  elif [ "${gnum}" -lt "${randnum}" ]; then
    echo "...bigger!";
  else
    echo "RIGHT! Guessed ${randnum} in ${tries} tries!";
    guessed=1;
  fi
done;
``````

**Зад 26. 05-b-7550**<br/>Да се напише shell скрипт, който приема параметър - име на потребител. Скриптът да прекратява изпълненито на всички текущо работещи процеси на дадения потребител, и да извежда колко са били те.

````bash
#!/bin/bash

if [ -z "${1}" ]; then
  echo "Enter user!";
  exit 1;
fi;

if [ $(who | grep -q "${1}" && echo 0 || echo 1) -eq 1 ]; then
   echo "${1} is not an logged user!"
   exit 1
fi

counter=0;

while read PID; do
   kill -15 "${PID}"
   sleep 5
   kill -9 "${PID}"
   counter=$(( $counter + 1 ));
done < <(ps -u "${1}" -o pid= );

echo "We killed ${counter} processes for the user ${1}";
````

**Зад 27. 05-b-7700**<br/>
Да се напише shell скрипт, който приема два параметъра - име на директория и число. Скриптът да извежда сумата от размерите на файловете в директорията, които имат размер, по-голям от подаденото число.

`````bash
#!/bin/bash

if [ ! -d "${1}" ]; then
  echo "Invalid directory!";
  exit 1;
fi;

if [[ ! "${2}" =~ ^[0-9]+$ ]]; then
  echo "Invalid number!";
  exit 1;
fi;

sumsize=0;

while read FILE; do
  fsize=$(stat "${FILE}" --printf='%s');
  if [ "${fsize}" -gt "${2}" ]; then
    echo "--> ${FILE} with size ${fsize}";
    sumsize=$(( $sumsize + $fsize ));
  fi
done < <(find "${1}" -type f 2>/dev/null);

echo "Total sum size is ${sumsize}";
`````

**Зад 28. 05-b-7800**<br/>Да се напише shell скрипт, който намира броя на изпълнимите файлове в PATH.
Hint: Предполага се, че няма спейсове в имената на директориите
Hint2: Ако все пак искаме да се справим с този случай, да се разгледа IFS променливата и констуркцията while read -d

````bash
#!/bin/bash

while IFS=':' read -d ':' LINE; do
  while read FILE; do
    if [ -r "${FILE}" ]; then
      echo "--> ${FILE} in ${LINE} is readable!";
    fi
  done < <(find "${LINE}" -type f 2>/dev/null);
done < <(echo "$PATH:");
````

**Зад 29. 05-b-8000**<br/>Напишете shell script, който получава като единствен аргумент име на потребител и за всеки негов процес изписва съобщение за съотношението на RSS към VSZ. Съобщенията да са сортирани, като процесите с най-много заета виртуална памет са най-отгоре.

`````bash
#!/bin/bash

if [ -z "${1}" ]; then
  echo "Enter user!";
  exit 1;
fi;

if [ $(who | grep -q "${1}" && echo 0 || echo 1) -eq 1 ]; then
   echo "${1} is not an logged user!"
   exit 1
fi

while read PID RSS VSZ; do
   echo "${PID}:"$(echo "scale=2; ${RSS}/${VSZ}" | bc -l |sed 's/^\./0./g');
done < <(ps -u "${1}" -o pid=,rss=,vsz= | head -n 10) | sort -t ':' -k 2 -r;
`````
**Зад 30. 05-b-9100**<br/>
Опишете поредица от команди или напишете shell скрипт, които/който при известни две директории SOURCE и DESTINATION:
= намира уникалните "разширения" на всички файлове, намиращи се някъде под SOURCE. (За простота приемаме, че в имената на файловете може да се среща символът точка '.' максимум веднъж.)
= за всяко "разширение" създава по една поддиректория на DESTINATION със същото име
= разпределя спрямо "разширението" всички файлове от SOURCE в съответните поддиректории в DESTINATION

````bash
#!/bin/bash

if [ ! -d "${1}" ]; then
   echo "${1} is not a directory.";
   exit 1;
fi

if [ ! -d "${2}" ]; then
   echo "${2} is not a directory.";
   exit 2;
fi

# find "${1}" -type f -printf '%f\n' 2>/dev/null | cut -d '.' -f 2
touch uniqExtensions allExtensions

exten=$(find "${1}" -type f -printf '%f\n' 2>/dev/null | cut -d '.' -f 2 | sort);
echo "${exten}" | uniq -u >> uniqExtensions;
echo "${exten}" | uniq >> allExtensions;

echo "Unique extensions are: $(cat uniqExtensions | tr '\n' ' ')";

while read EXTENSION; do
  mkdir "${EXTENSION}";
  mv "${EXTENSION}" "${2}";
done < allExtensions;

while read FILE; do
  fileexten=$(echo "${FILE}" | cut -d '.' -f 2);
  cp "${FILE}" "${2}/${fileexten}/$(basename $FILE)";
done < <(find "${1}" -type f -printf '%p\n' 2>/dev/null);

rm uniqExtensions allExtensions
````

**Зад 31. 05-b-9200**<br/>Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.
За всеки изтрит файл (директория) скриптът добавя ред във log файл с подходящо съобщение.

а) Името на log файла да се чете от shell environment променлива, която сте конфигурирали във вашия .bashrc.
б) Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно.
в) Добавете timestamp на log съобщенията във формата: 2018-05-01 22:51:36

`````bash
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "No arguments!";
  exit 1;
fi;

remrec=0;

while [ -n "${1}" ]; do
  if [ "${1}" = "-r" ]; then
    remrec=1;
  fi

  if [ -f "${1}" ]; then
    rm "${1}";
    echo "[$(date +'%Y-%m %H:%M:%S')] Removed file ${1}" >> "${RMLOG_FILE}"
  elif [ -d "${1}" ]; then
    if [ $remrec -eq 0 ]; then
      if [ $(ls "${1}" | wc -l) -eq 0 ]; then
        rmdir "${1}";
        echo "[$(date +'%Y-%m %H:%M:%S')] Removed directory ${1}" >> "${RMLOG_FILE}"
      fi
    else
      # remove dir recursively
      rm -r  "${1}"
            echo "[$(date +'%Y-%m %H:%M:%S')] Removed directory recursively ${1}" >> "${RMLOG_FILE}"
    fi
  fi;
  shift;
done;
`````

**Зад 32. 05-b-9500**<br/>(Цветно принтиране) Напишете shell script color_print, който взима два параметъра.

Първият може да е измежду "-r", "-g" "-b", а вторият е произволен string.
На командата "echo" може да се подаде код на цвят, който ще оцвети текста в определения цвят.
В зависимост от първия аргумент, изпринтете втория аргумен в определения цвят:

"-r" е червено. Кодът на червеното е '\033[0;31m' (echo -e "\033[0;31m This is red")
"-g" е зелено. Кодът на зеленото е '\033[0;32m' (echo -e "\033[0;32m This is green")
"-b" е синьо. Кодът на синьото е '\033[0;34m' (echo -e "\033[0;34m This is blue")
Ако е подадена друга буква изпишете "Unknown colour", а ако изобщо не е подаден аргумент за цвят, просто изпишете текста.

````bash
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "No arguments";
  exit 1;
fi;

color="";

case "${1}" in
  "-r")
    color="\033[0;31m";
    # echo -e "\033[0;31m ${2}";
    ;;
  "-g")
    color="\033[0;32m";
    ;;
  "-b")
    color="\033[0;34m";
    ;;
  *)
    echo "Unknown colour";
    color="\033[0m";
    ;;
esac

if [ "${1}" = "-r" ] || [ "${1}" = "-g" ] || [ "${1}" = "-b" ]; then
  echo -e "${color}${2}";
else
  echo "${1}";
fi

echo -e "\033[0m";
````

**Зад 33. 05-b-9501**<br/>Този път програмата ви ще приема само един параметър, който е измежду ("-r", "-b", "-g", "-x").
Напишете shell script, който приема редовете от stdin и ги изпринтва всеки ред с редуващ се цвят. Цветовете вървят RED-GREEN-BLUE и цветът на първия ред се определя от аргумента.
 Ако е подаден аргумент "-x", то не трябва да променяте цветовете в терминала (т.е., все едно сте извикали командата cat).

````bash
#!/bin/bash

counter=0;
if [ "${1}" = "-r" ]; then
  counter=1;
elif [ "${1}" = "-g" ]; then
  counter=2;
elif [ "${1}" = "-b" ]; then
  counter=3;
elif [ "${1}" = "-x" ]; then
  counter=4;
else
  echo "Invalid agument!";
  exit 1;
fi;

if [ $counter -lt 4 ]; then
  while read LINE; do
    if [ $counter -eq 1 ]; then
      echo -e "\033[0;31m${LINE}\033[0m";
    elif [ $counter -eq 2 ]; then
      echo -e "\033[0;32m${LINE}\033[0m";
    elif [ $counter -eq 3 ]; then
      echo -e "\033[0;34m${LINE}\033[0m";
      counter=0;
    fi;
    counter=$(( $counter + 1 ));
  done;
else
  while read LINE; do
    echo "${LINE}";
  done;
fi;

echo -e "\033[0m";
````

**Зад 34. 05-b-9600**<br/>Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.

`````bash
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "No arguments!";
  exit 1;
fi;

if [ -z "${BACKUP_DIR}" ]; then 
  if [ ! -d "./backup" ]; then
    mkdir ./backup
  fi
  BACKUP_DIR="./backup"
fi

remrec=0;

while [ -n "${1}" ]; do
  if [ "${1}" = "-r" ]; then
    remrec=1;
    shift;
  fi

  temp=$(echo "${1}_$(date +'%Y_%m_%d_%H_%M_%S')");

  if [ -f "${1}" ]; then
        tar -cf "${temp}.tar" "${1}"
        gzip "${temp}.tar"
        mv "${temp}.tar.gz" "${BACKUP_DIR}"

        rm "${1}";
  elif [ -d "${1}" ]; then
    if [ $remrec -eq 0 ]; then
      if [ $(ls "${1}" | wc -l) -eq 0 ]; then
              tar -cf "${temp}.tar" "${1}"
              gzip "${temp}.tar"
              mv "${temp}.tar.gz" "${BACKUP_DIR}"

        rmdir "${1}";
          else
            echo "Directory ${1} will not be deleted because is not empty!";
      fi
    else
      # Remove directory recursively
            tar -cf "${temp}.tar" "${1}"
            gzip "${temp}.tar"
            mv "${temp}.tar.gz" "${BACKUP_DIR}"

            rm -r "${1}";
    fi
  fi;
  shift;
done;
`````

**Зад 35. 05-b-9601**<br/>Да се напише shell скрипт, който възстановява изтрити файлове, които имат запазено копие в BACKUP_DIR (от предната задача).
При възстановяването файловете да се декомпресират, а директориите да се декомпресират и разархивират.

а) Да се дефинира параметър -l, който изрежда всички файлове, които могат да бъдат възстановени и датата на тяхното изтриване.

б) Скриптът да приема 2 параметъра. Първият е името на файла, който да се възстанови, а вторията е директорията, в която файлът да бъде възстановен. Ако вторият аргумент липсва, файлът да се възстановява в сегашната директория, където скриптът се изпълнява.

в) Когато има N > 1 запазени файла със същото име, да се изпише списък с N реда на потребителя и да се изиска той да въведе цяло число от 1 до N, за да избере кой файл да възстанови.

````bash
#!/bin/bash

if [ -z "${BACKUP_DIR}" ]; then 
  BACKUP_DIR="./backup"
fi

if [ ! -d "${BACKUP_DIR}" ]; then 
  echo "Invalid backup directory!";
  exit 1;
fi

while read LINE; do
   path=$(echo "${LINE}" | cut -d ' ' -f1);
   file=$(echo "${LINE}" | cut -d ' ' -f2);
   if [[ "${file}" =~ \.tar\.gz$ ]]; then
       gzip -d "${file}"
       newfile=$(echo "${file}" | cut -d '.' -f1,2)  
       tar -xf "${newfile}"
   elif [[ "${file}" =~ \.gz$ ]]; then
      gzip -d "${file}"
    fi
done < <(find "${BACKUP_DIR}" -printf "%p %f\n")


stat -c '%x' fileName
````


