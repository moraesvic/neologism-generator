APPJS=QlpoOTFBWSZTWV4BjCkAAE9fgEAQQO3CChgk1gC///9OMADSsGohoAAAADQANA0RpDEmjTTaQGgekaaMmgSSjSeiaPU9TQGgAAAwqd5j6GEozJCUPjn2Ozw2JjGZoFbhFa8DJKGnBrr7EZAgb57rjbQBV+NpkpA0SJRiNAXnLXScy75OAtntABSDPDHFc4CofjSOBmAZTSdXIRkRz7uYmxVyMICEHrLUwEOMaYGT9MsxOWixSIs3SyjJQyX1goanBENARo2zFVGcqFpCWZjArVJLSDzUOPbE6c0KU8Zo5pAZF4jMhzGFQKO5aUPXQshtLhdyRThQkF4BjCk=
echo $APPJS | base64 -d | bzip2 -d > app.js

mkdir -p public

npm init -y
npm i express

git init
git add .
git commit -m 'first commit'

rm install.sh