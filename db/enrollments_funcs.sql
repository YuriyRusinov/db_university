drop type if exists dbenrollments cascade;
create type dbenrollments as (
    id_enrollment integer,
    enrollment_uuid uuid,
    student_id integer,
    course_id integer,
    enrollment_date date,
    grade numeric(4, 2),
    status varchar,
    semester varchar,
    credit_hours integer
);

create or replace function get_enrollments(id_enrollment integer) returns setof dbenrollments as
$BODY$
declare
    query varchar;
    r dbenrollments%rowtype;
begin
    query := E'select e.id, e.uuid, e.student_id, e.course_id, e.enrollment_date, e.grade, e.status, e.semester, e.credit_hours from enrollments e';
    if( id_enrollment is not null ) then
        query := query || E' where e.id = ' || id_enrollment;
    end if;

    for r in
        execute query
    loop
        return next r;
    end loop;
    return;
end
$BODY$
language 'plpgsql';

create or replace function add_student_to_course ( id_student integer,
    id_course integer,
    e_enrollment_date date, e_grade numeric(4,2),
    e_status varchar default 'active', 
    e_semester varchar default '',
    e_hours integer default 0) returns integer as
$BODY$
declare
    id_enrollment integer;
    cnt integer;
    query varchar;
begin
    if( id_student is null or id_course is null or e_enrollment_date is null or e_semester is null ) then
        raise exception 'Student, course, enrollment date and semester cannot be null';
    end if;
    query := E'select  count(*) from enrollments e where e.student_id = ' || id_student || E' and e.course_id = ' ||  id_course || E' and lower(e.semester) = lower(\'' || e_semester || E'\');';
    execute query into cnt;
    raise warning '%', cnt;
    if( cnt > 0 ) then
        raise warning 'Student % already enrolled to course %', id_student, id_course;
        return -1;
    end if;
    select into cnt count(*) from courses c where c.id = id_course and c.is_active;
    if( cnt = 0 ) then
        raise warning 'Course % is not present in table courses or is not active', id_course;
        return -2;
    end if;
    select into cnt count(*) from students s where s.id = id_student and s.status = 'active';
    if( cnt = 0 ) then
        raise warning 'Student % is not present in table students or is not active', id_student;
        return -3;
    end if;

    select into id_enrollment nextval('enrollments_id_seq'::regclass);
    query := E'insert into enrollments( id, student_id, course_id, enrollment_date, grade, status, semester, credit_hours ) values (';
    query := query || id_enrollment || E', ' || id_student || E', ' || id_course ||
        E', \'' || e_enrollment_date || E'\'::date, ';
    if( e_grade is null ) then
        query := query || E'null::numeric(4,2), ';
    else
        query := query || e_grade || E'::numeric(4,2), ';
    end if;
    query := query || E'\'' || e_status || E'\', \'' || e_semester || E'\', ' || e_hours || E');';
    raise warning ' query %', query;
    execute query;

    return id_enrollment;
    exception when others then
        raise warning '%', query;
        return -4;
end
$BODY$
language 'plpgsql';

create or replace function update_course_student_status(id_student integer, id_course integer, e_semester varchar, e_status varchar default 'active') returns integer as
$BODY$
declare
    id_enrollment integer;

    query varchar;
begin

    if( id_student is null or id_course is null or e_semester is null ) then
        raise warning 'Student ID or course ID cannot be null';
        return -1;
    end if;
    select into id_enrollment e.id from enrollments e where e.student_id = id_student and e.course_id = id_course and lower(e.semester) = lower(e_semester);
    if( id_enrollment is null ) then
        raise warning 'Cannot find student % on course %', id_student, id_course;
        return -2;
    end if;
    update enrollments set status = e_status where id=id_enrollment;

    return id_enrollment;
    exception when others then
        raise warning '%', query;
        return -4;
end
$BODY$
language 'plpgsql';

create or replace function update_course_student_semester( id_student integer,
    id_course integer,
    e_enrollment_date date, e_grade numeric(4,2),
    e_status varchar,
    e_semester varchar,
    e_hours integer default 0) returns integer as
$BODY$
declare
    id_enrollment integer;
begin
    if( id_student is null or id_course is null or e_semester is null or e_enrollment_date is null ) then
        raise warning 'Student, course, semester, enrollment date cannot be null';
        return -1;
    end if;

    select into id_enrollment e.id from enrollments e where e.student_id = id_student and e.course_id = id_course and lower(e.semester) = lower(e_semester);
    if( id_enrollment is null ) then
        id_enrollment := add_student_to_course( id_student, id_course, e_enrollment_date, e_grade, e_status, e_semester, e_hours );
        return id_enrollment;
    end if;

    update enrollments set enrollment_date = e_enrollment_date, grade = e_grade, status = e_status, credit_hours = e_hours where id = id_enrollment;
    return id_enrollment;
    exception when others then
        return -4;
end
$BODY$
language 'plpgsql';

create or replace function drop_student_from_course( id_student integer, id_course integer, semester varchar ) returns integer as
$BODY$
declare
    id_enrollment integer;
begin
    if( id_student is null or id_course is null or e_semester is null ) then
        raise warning 'Student, course, semester cannot be null';
        return -1;
    end if;

    select into id_enrollment e.id from enrollments e where e.student_id = id_student and e.course_id = id_course and lower(e.semester) = lower(e_semester);
    if( id_enrollment is null ) then
        raise warning 'Cannot find student % on course %', id_student, id_course;
        return -2;
    end if;
    delete from enrollments where id = id_enrollment;
    return id_enrollment;
    exception when others then
        return -4;
end
$BODY$
language 'plpgsql';
