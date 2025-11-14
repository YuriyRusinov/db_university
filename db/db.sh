# /bin/bash

DBNAME=$1
HOST=$2
if [[ $# -ge 3 ]]; then
    PORT=$3
else
    PORT=5432
fi
if [[ $# -ge 4 ]]; then
    USER=$4
else
    USER=postgres
fi

/usr/bin/createdb --host=$HOST --port=$PORT --username=$USER --owner=$USER $DBNAME
/usr/bin/psql -f create_students_db.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f students_db_data.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f students_db_funcs.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f students_profile.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f departments.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f courses.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
/usr/bin/psql -f enrollments.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
#/usr/bin/psql -f students_db_view.sql --host=$HOST --port=$PORT --username=$USER $DBNAME
