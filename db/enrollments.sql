create or replace function enroll_student_to_course(p_id_student integer, p_id_course integer, p_semester varchar, p_enrollment_date date default current_date, p_status varchar default 'active') returns integer as
$BODY$
declare
    query varchar;
    id_enrollment integer;

    is_student_exists boolean;
    is_course_exists boolean;
    course_credits integer;
    current_enrollment_count integer;
    course_capacity integer;
    max_capacity integer;
    res integer;
begin
    raise warning '% % % % %', p_id_student, p_id_course, p_semester, p_enrollment_date, p_status;
    select exists (select 1 from students s where s.id=p_id_student and s.status='active') into is_student_exists;
    raise warning '%', is_student_exists;

    if( not is_student_exists ) then
        raise warning 'student % is not present or is not active', p_id_student;
        return -1;
    end if;
    select exists (select 1 from courses c where c.id=p_id_course and c.is_active) into is_course_exists;
    if( not is_course_exists ) then
        raise warning 'course % is not present or is not active', p_id_course;
        return -2;
    end if;
    select into course_capacity c.max_capacity from courses c where c.id=p_id_course and c.is_active;
    if exists (
        SELECT 1 FROM Enrollments 
        WHERE student_id = p_id_student 
        AND course_id = p_id_course 
        AND semester = p_semester
        AND status != 'withdrawn'
    ) then
        raise warning 'Student % already enrolled to course %', p_id_student, p_id_course;
        return -3;
    end if;

    select into current_enrollment_count count(*) from enrollments e where e.course_id = p_id_course and e.semester = p_semester and status='active';
    if( current_enrollment_count >= course_capacity ) then
        return -4;
    end if;

    select into course_credits c.credits from courses c where c.id=p_id_course;
    raise warning 'credit course %', course_credits;

    select into id_enrollment nextval('enrollments_id_seq'::regclass);
    query := E'insert into enrollments (id, student_id, course_id, enrollment_date, semester, credit_hours, status ) VALUES (' || id_enrollment || E',' || p_id_student || E',' || p_id_course || E', \'' || p_enrollment_date || E'\'::date, \'' || p_semester || E'\',';
    raise warning '%', query;
    query := query || course_credits || E', \'active\'::varchar)';
    raise warning 'query %', query;
    execute query;

    return id_enrollment;
    exception when others then
        raise warning '%', query;
        return -5;
end
$BODY$
language 'plpgsql';

create or replace function withdraw_student_from_course(p_id_student integer, p_id_course integer, p_semester varchar) returns integer as
$BODY$
declare
    cnt integer;
    id_enrollment integer;
begin
    select into id_enrollment e.id from enrollments e where e.student_id=p_id_student and e.course_id=p_id_course and e.semester = p_semester and e.status = 'active' and e.status <> 'completed';
    if( id_enrollment is null ) then
        raise warning 'Student % not found on course %', p_id_student, p_id_course;
        return -1;
    end if;
    update enrollments set status='withdrawn' where id=id_enrollment;

    return id_enrollment;
end
$BODY$
language 'plpgsql';

create or replace function update_student_on_course(p_id_student integer, p_id_course integer, p_semester varchar, p_status varchar) returns integer as
$BODY$
declare
    cnt integer;
    id_enrollment integer;
begin
    select into id_enrollment e.id from enrollments e where e.student_id=p_id_student and e.course_id=p_id_course and e.semester = p_semester ;
    if( id_enrollment is null ) then
        raise warning 'Student % not found on course %', p_id_student, p_id_course;
        return -1;
    end if;
    update enrollments set status=p_status where id=id_enrollment;

    return id_enrollment;
end
$BODY$
language 'plpgsql';


