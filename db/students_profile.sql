drop type if exists studentprofile cascade;
create type studentprofile as (
    id integer,
    id_student integer,
    academic_standing varchar,
    financial_balance double precision,
    student_uuid uuid,
    phone_number varchar,
    emergency_contact_name varchar,
    emergency_contact_phone varchar,
    address text,
    city varchar,
    state varchar,
    postal_code varchar,
    country varchar,
    medical_notes varchar
);

create or replace function get_student_profile(id_student integer) returns setof studentprofile as
$BODY$
declare
    query varchar;
    r studentprofile%rowtype;
begin
    query := E'select sp.id, sp.student_id, sp.academic_standing, sp.financial_balance, sp.uuid, sp.phone_number, sp.emergency_contact_name, sp.emergency_contact_phone, sp.address, sp.city, sp.state, sp.postal_code, sp.country, sp.medical_notes from student_profiles sp where sp.student_id = ';
    if( id_student is null ) then
        query := query || E'null::integer';
    else
        query := query || id_student;
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
