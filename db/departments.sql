drop type if exists db_departments cascade;
create type db_departments as (
    id_department integer,
    department_uuid uuid,
    department_code varchar,
    department_name varchar,
    chair_person varchar,
    location varchar,
    budget numeric(12,2),
    established_date date,
    is_active boolean
);

create or replace function get_departments(integer) returns setof db_departments as
$BODY$
declare
    id_dept alias for $1;
    query varchar;
    r db_departments%rowtype;
begin

    query := E'select d.id, d.uuid, d.department_code, d.department_name, d.chair_person, d.office_location, d.budget, d.established_date, d.is_active from departments d';
    if( id_dept is not null ) then
        query := query || E' where id=' || id_dept;
    end if;
    query := query || E' order by 4 asc';
    for r in
        execute query
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

create or replace function add_department(varchar, varchar, varchar, varchar, numeric(12,2), date, boolean) returns integer as
$BODY$
declare
    code alias for $1;
    name alias for $2;
    chair alias for $3;
    location alias for $4;
    depbudget alias for $5;
    est_date alias for $6;
    dep_active alias for $7;

    query varchar;
    id_dep integer;
begin
    select into id_dep nextval('departments_id_seq'::regclass);
    query := E'insert into departments(id, department_code, department_name, chair_person, office_location, budget, established_date, is_active) values (' || id_dep || E',';
    if( code is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || code || E'\'';
    end if;
    query := query || E', ';
    if( name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || name || E'\'';
    end if;
    query := query || E', ';
    if( chair is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || chair || E'\'';
    end if;
    query := query || E', ';
    if( location is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || location || E'\'';
    end if;
    query := query || E', ';
    if( depbudget is null ) then
        query := query || E'null::numeric(12,2)';
    else
        query := query || E'\'' || depbudget || E'\'::numeric(12,2)';
    end if;
    query := query || E', ';
    if( est_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || est_date || E'\'::date';
    end if;
    query := query || E', ';
    if( dep_active is null ) then
        query := query || E'null::boolean';
    else
        query := query || E'\'' || dep_active || E'\'::boolean';
    end if;
    query := query || E')';
    raise warning '%', query;
    execute query;

    return id_dep;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function update_department(integer, varchar, varchar, varchar, varchar, numeric(12,2), date, boolean) returns integer as
$BODY$
declare
    id_dep alias for $1;
    code alias for $2;
    name alias for $3;
    chair alias for $4;
    location alias for $5;
    depbudget alias for $6;
    est_date alias for $7;
    dep_active alias for $8;

    query varchar;
begin
    query := E'update departments set department_code = ';
    if( code is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || code || E'\'';
    end if;
    query := query || E', department_name=';
    if( name is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || name || E'\'';
    end if;
    query := query || E', chair_person=';
    if( chair is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || chair || E'\'';
    end if;
    query := query || E', office_location=';
    if( location is null ) then
        query := query || E'null::varchar';
    else
        query := query || E'\'' || location || E'\'';
    end if;
    query := query || E', budget=';
    if( depbudget is null ) then
        query := query || E'null::numeric(12,2)';
    else
        query := query || E'\'' || depbudget || E'\'::numeric(12,2)';
    end if;
    query := query || E', established_date=';
    if( est_date is null ) then
        query := query || E'null::date';
    else
        query := query || E'\'' || est_date || E'\'::date';
    end if;
    query := query || E', is_active=';
    if( dep_active is null ) then
        query := query || E'null::boolean';
    else
        query := query || E'\'' || dep_active || E'\'::boolean';
    end if;
    query := query || E' where id=' || id_dep || E';';
    raise warning '%', query;
    execute query;

    return id_dep;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';

create or replace function delete_department(integer) returns integer as
$BODY$
declare
    id_dep alias for $1;
begin
    delete from departments where id = id_dep;
    return id_dep;
    exception when others then
        return -1;
end
$BODY$
language 'plpgsql';
