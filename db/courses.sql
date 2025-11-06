drop type if exists db_courses cascade;
create type db_courses as (
    id_course integer,
    uuid_course uuid,
    code varchar,
    name varchar,
    description text,
    mark integer,
    id_department integer,
    department_code varchar,
    department_name varchar,
    professor_name varchar,
    max_capacity integer,
    is_course_active boolean
);

create or replace function get_courses(integer) returns setof db_courses as
$BODY$
declare
    id_course alias for $1;

    r db_courses%rowtype;
    query varchar;
begin

    query := E'select c.id, c.uuid, c.course_code, c.course_name, c.description, c.credits, d.id, d.department_code, d.department_name, c.instructor_name, c.max_capacity, c.is_active from courses c inner join departments d on (c.department_id = d.id';
    if(id_course is null) then
        query := query || E')';
    else
        query := query || E' and c.id=' || id_course || E')';
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

create or replace function add_course(varchar, varchar, text, integer, integer, varchar, integer, boolean) returns integer as
$BODY$
declare
    ccode alias for $1;
    cname alias for $2;
    m_desc alias for $3;
    m_credits alias for $4;
    m_dep_id alias for $5;
    m_dean alias for $6;
    m_capacity alias for $7;
    m_is_active alias for $8;

    id_course integer;
    query varchar;
    cnt integer;
begin
    select into cnt count(c.course_code) from courses c where c.course_code = ccode;
    if( cnt > 0) then
        raise warning 'not unique code %', ccode;
        return -1;
    end if;
    select into id_course nextval('courses_id_seq'::regclass);
    query := E'insert into courses (id, course_code, course_name, description, credits, department_id, instructor_name, max_capacity, is_active) values (' || id_course;
    if( ccode is null ) then
        query := query || E'null::varchar';
    else
        query := query || E', \'' || ccode || E'\'';
    end if;
    query := query || E', ';
    if( cname is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || cname || E'\'';
    end if;
    query := query || E', ';
    if( m_desc is null ) then
        query := query || E'null::text';
    else
        query := query || E'\'' || m_desc || E'\'';
    end if;
    query := query || E', ';
    if( m_credits is null ) then
        query := query || E'null::integer';
    else
        query := query || m_credits;
    end if;
    query := query || E', ';
    if( m_dep_id is null ) then
        query := query || E'null::integer';
    else
        query := query || m_dep_id;
    end if;
    query := query || E', ';
    if( m_dean is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_dean || E'\'';
    end if;
    query := query || E', ';
    if( m_capacity is null ) then
        query := query || E'null::integer';
    else
        query := query || m_capacity;
    end if;
    query := query || E', ';
    if( m_is_active is null or m_is_active ) then
        query := query || E'true';
    else
        if ( not m_is_active ) then
            query := query || E'false';
        end if;
    end if;
    query := query || E')';

    raise warning '%', query;
    execute query;

    return id_course;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function update_course(integer, varchar, varchar, text, integer, integer, varchar, integer, boolean) returns integer as
$BODY$
declare
    id_course alias for $1;
    ccode alias for $2;
    cname alias for $3;
    m_desc alias for $4;
    m_credits alias for $5;
    m_dep_id alias for $6;
    m_dean alias for $7;
    m_capacity alias for $8;
    m_is_active alias for $9;

    query varchar;
    cnt integer;
begin
    query := E'update courses set course_code =';
    if( ccode is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || ccode || E'\'';
    end if;
    query := query || E', course_name=';
    if( cname is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || cname || E'\'';
    end if;
    query := query || E', description=';
    if( m_desc is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_desc || E'\'';
    end if;
    query := query || E', credits=';
    if( m_credits is null ) then
        query := query || E'null::integer';
    else
        query := query || m_credits ;
    end if;
    query := query || E', department_id=';
    if( m_dep_id is null ) then
        query := query || E'null::integer';
    else
        query := query || m_dep_id;
    end if;
    query := query || E', instructor_name=';
    if( m_dean is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || m_dean || E'\'';
    end if;
    query := query || E', max_capacity=';
    if( m_capacity is null ) then
        query := query || E'null::integer';
    else
        query := query || m_capacity;
    end if;
    query := query || E', is_active=';
    if( m_is_active is null or m_is_active ) then
        query := query || E'true';
    else
        query := query || E'false';
    end if;
    query := query || E' where id = ' || id_course;
    execute query;

    return id_course;
    exception when others then
        raise warning 'Invalid query %', query;
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function delete_course(integer) returns integer as
$BODY$
declare
    id_course alias for $1;
begin
    delete from courses where id=id_course;

    return id_course;
    exception when others then
        raise warning 'Invalid query %', query;
        return -1;
end
$BODY$
language 'plpgsql';
