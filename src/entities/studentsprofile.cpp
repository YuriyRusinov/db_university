#include "studentsprofile.hpp"

StudentProfile::StudentProfile(
        int id,
        int student_id,
        AcademicStanding academic_standing,
        double financialBalance,
        const std::string& phone_number,
        const std::string& emergency_contact_name,
        const std::string& emergency_contact_phone,
        const std::string& address,
        const std::string& city,
        const std::string& state,
        const std::string& postal_code,
        const std::string& country,
        const std::string& medical_notes
    )
    : m_id( id ),
    m_student_id( student_id ),
    m_academic_standing( academic_standing ),
    m_financialBalance( financialBalance ),
    m_phone_number( phone_number.empty() ? std::nullopt : std::make_optional( std::move( phone_number ) ) ),
    m_emergency_contact_name( emergency_contact_name.empty() ? std::nullopt : std::make_optional( std::move(  emergency_contact_name ) ) ),
    m_emergency_contact_phone( emergency_contact_phone.empty() ? std::nullopt : std::make_optional( std::move( emergency_contact_phone ) ) ),
    m_address( address.empty() ? std::nullopt : std::make_optional( std::move( address ) ) ),
    m_city( city.empty() ? std::nullopt : std::make_optional( std::move( city ) ) ),
    m_state( state.empty() ? std::nullopt : std::make_optional( std::move( state ) ) ),
    m_postal_code( postal_code.empty() ? std::nullopt : std::make_optional( std::move( postal_code ) ) ),
    m_country( country.empty() ? std::nullopt : std::make_optional( std::move( country ) ) ),
    m_medical_notes( medical_notes.empty() ? std::nullopt : std::make_optional( std::move( medical_notes ) ) )
{
}

std::string StudentProfile::getFullAcademicStanding() const {
    switch( m_academic_standing ) {
        case AcademicStanding::Unknown: default: return std::string(); break;
        case AcademicStanding::Good: return std::string("good"); break;
        case AcademicStanding::Probation: return std::string("probation"); break;
        case AcademicStanding::Warning: return std::string("warning"); break;
        case AcademicStanding::Suspended: return std::string("suspended"); break;
        case AcademicStanding::Graduated: return std::string("graduated"); break;
    }
}
