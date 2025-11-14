#include "studentsprofile.hpp"

StudentProfile::StudentProfile(
        int id,
        int student_id,
        AcademicStanding academic_standing,
        double financialBalance,
        std::optional<std::string> phone_number,
        std::optional<std::string> emergency_contact_name,
        std::optional<std::string> emergency_contact_phone,
        std::optional<std::string> address,
        std::optional<std::string> city,
        std::optional<std::string> state,
        std::optional<std::string> postal_code,
        std::optional<std::string> country,
        std::optional<std::string> medical_notes
    )
    : m_id( id ),
    m_student_id( student_id ),
    m_academic_standing( academic_standing ),
    m_financialBalance( financialBalance ),
    m_phone_number( phone_number ),
    m_emergency_contact_name( emergency_contact_name ),
    m_emergency_contact_phone( emergency_contact_phone ),
    m_address( address ),
    m_city( city ),
    m_state( state ),
    m_postal_code( postal_code ),
    m_country( country ),
    m_medical_notes( medical_notes )
{

}
