create extension if not exists plpgsql;
drop type if exists dbstudents cascade;
create type dbstudents as (
    id_student integer,
    student_uuid uuid,
    student_number varchar,
    first_name varchar,
    middle_name varchar,
    last_name varchar,
    email varchar,
    date_of_birth date,
    enrollment_date date,
    graduation_date date,
    status varchar,
    phone_number varchar,
    emergency_contact_name varchar,
    emergency_contact_phone varchar,
    address text,
    city varchar,
    state varchar,
    postal_code varchar,
    country varchar,
    academic_standing varchar,
    financial_balance varchar
);

create or replace function getstudents(integer) returns setof dbstudents as
$BODY$
declare
    id_student alias for $1;
    query varchar;
    r dbstudents%rowtype;
begin
    query := E'select s.id, s.uuid, s.student_number, s.first_name, s.middle_name, s.last_name, s.email, s.date_of_birth, s.enrollment_date, s.graduation_date, s.status, sp.phone_number, sp.emergency_contact_name, sp.emergency_contact_phone, sp.address, sp.city, sp.state, sp.postal_code, sp.country, sp.academic_standing, sp.financial_balance from students s left join student_profiles sp on (s.id = sp.student_id) ';
    if( id_student is not null ) then
        query := query || E' where s.id = ' || id_student || E' ';
    end if;
    query := query || E' order by 6 asc';
    raise warning '%', query;
    for r in
        execute query
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function insert_student(varchar, varchar, varchar, varchar, date, date, date, varchar) returns integer as
$BODY$
declare
    m_first_name alias for $1;
    m_middle_name alias for $2;
    m_last_name alias for $3;
    m_email alias for $4;
    m_birth_date alias for $5;
    m_enroll_date alias for $6;
    m_graduate_date alias for $7;
    m_status alias for $8;

    query varchar;
    id_student integer;
    student_uuid uuid;
    id_student_profile integer;
begin
    select into id_student nextval('students_id_seq'::regclass);
    query := E'insert into students(id, first_name, middle_name, last_name, email, date_of_birth, enrollment_date, graduation_date, status) values (' || id_student || E', ';
    if( m_first_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_first_name || E'\'';
    end if;
    query := query || E', ';
    if( m_middle_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\''|| m_middle_name || E'\'';
    end if;
    query := query || E', ';
    if( m_last_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_last_name || E'\'';
    end if;
    query := query || E', ';
    if( m_email is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_email || E'\'';
    end if;
    query := query || E', ';

    if( m_birth_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || m_birth_date || E'\'';
    end if;
    query := query || E', ';
    if( m_enroll_date is null ) then
        query := query || E'null::date';
    else
        query := query ||  E'\'' || m_enroll_date || E'\'';
    end if;
    query := query || E', ';
    if( m_graduate_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || m_graduate_date || E'\'';
    end if;
    query := query || E', ';
    if( m_status is null ) then
        query := query || E'\'active\'';
    else
        query := query || E'\'' || m_status || E'\'';
    end if;
    query := query || E');';
    raise warning '%', query;
    execute query;

    select into id_student_profile nextval('student_profiles_id_seq'::regclass);
    raise warning 'id_student_profile %', id_student_profile;

    insert into student_profiles(id, student_id) values( id_student_profile, id_student );

    return id_student;
    exception when others then
        raise exception 'incorrect query %', query;
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function update_student(p_id_student integer, m_first_name varchar, m_middle_name varchar,  m_last_name varchar, m_email varchar, m_birth_date date, m_enroll_date date, m_graduate_date date, m_status varchar default 'active') returns integer as
$BODY$
declare

    query varchar;
    id_student integer;
    student_uuid uuid;
    id_student_profile integer;
begin
    query := E'update students set first_name = ';----    , ) values (' || id_student || E', ';
    if( m_first_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_first_name || E'\'';
    end if;
    query := query || E', middle_name =';
    if( m_middle_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\''|| m_middle_name || E'\'';
    end if;
    query := query || E', last_name=';
    if( m_last_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_last_name || E'\'';
    end if;
    query := query || E', email=';
    if( m_email is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_email || E'\'';
    end if;
    query := query || E', date_of_birth=';

    if( m_birth_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || m_birth_date || E'\'::date';
    end if;
    query := query || E', enrollment_date=';
    if( m_enroll_date is null ) then
        query := query || E'null::date';
    else
        query := query ||  E'\'' || m_enroll_date || E'\'::date';
    end if;
    query := query || E', graduation_date=';
    if( m_graduate_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || m_graduate_date || E'\'::date';
    end if;
    query := query || E', status=';
    if( m_status is null ) then
        query := query || E'\'active\'';
    else
        query := query || E'\'' || m_status || E'\'';
    end if;
    query := query || E' where id=' || p_id_student;
    raise warning '%', query;
    execute query;

    return p_id_student;
    exception when others then
        raise exception 'incorrect query %', query;
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function update_student_profile(integer, varchar, varchar, varchar, text, varchar, varchar, varchar, varchar, text, varchar, numeric(8, 2)) returns integer as
$BODY$
declare
    id_student alias for $1;
    m_phone_number alias for $2;
    m_emergency_contact_name alias for $3;
    m_emergency_contact_phone alias for $4;
    m_address alias for $5;
    m_city alias for $6;
    m_state alias for $7;
    m_postal_code alias for $8;
    m_country alias for $9;
    m_medical_notes alias for $10;
    m_academic_standing alias for $11;
    m_financial_balance alias for $12;

    query varchar;
    cnt integer;
begin
    select into cnt count(*) from student_profiles sp where sp.student_id = id_student;
    if( cnt <> 1) then
        raise warning 'cannot find student % in list', id_student;
        return -1;
    end if;

    query := E'update student_profiles set phone_number = ';
    if( m_phone_number is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_phone_number || E'\'';
    end if;
    query := query || E', emergency_contact_name = ';
    if( m_emergency_contact_name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_emergency_contact_name || E'\'';
    end if;
    query := query || E', emergency_contact_phone =';
    if( m_emergency_contact_phone is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_emergency_contact_phone || E'\'';
    end if;
    query := query || E', address = ';
    if( m_address is null ) then
        query := query || E'null::text';
    else
        query := query || E'\'' || m_address || E'\'::text';
    end if;
    query := query || E', city = ';
    if( m_city is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_city || E'\'::varchar';
    end if;
    query := query || E', state =';
    if( m_state is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_state || E'\'::varchar';
    end if;
    query := query || E', postal_code=';
    if( m_postal_code is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_postal_code || E'\'::varchar';
    end if;
    query := query || E', country=';
    if( m_country is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_country || E'\'::varchar';
    end if;
    query := query || E', medical_notes = ';
    if( m_medical_notes is null ) then
        query := query || E'null::text';
    else
        query := query || E'\'' || m_medical_notes || E'\'::text';
    end if;
    query := query || E', academic_standing = ';
    if( m_academic_standing is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_academic_standing || E'\'::varchar';
    end if;
    query := query || E', financial_balance = ';
    if( m_financial_balance is null ) then
        query := query || E'null::numeric(8,2)';
    else
        query := query || E'\'' || m_financial_balance  || E'\'::numeric(8,2)';
    end if;
    query := query || ' where student_id = ' || id_student;
    raise warning '%', query;
    execute query;

    return id_student;

    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function delete_student(integer) returns integer as
$BODY$
declare
    id_student alias for $1;
begin
    delete from student_profiles where student_id = id_student;
    delete from students where id = id_student;

    return id_student;
end
$BODY$
language 'plpgsql';
