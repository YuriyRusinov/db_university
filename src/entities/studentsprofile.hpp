#pragma once

#include <optional>
#include <string>

enum class AcademicStanding {
    Unknown = -1,
    Good = 0,       // Meeting all academic requirements
    Probation = 1,  // Below academic standards, needs improvement
    Warning = 2,    // Academic performance concerns
    Suspended = 3,  // Temporarily barred from enrollment
    Graduated = 4   // Graduated
};

class StudentProfile {
public:
    StudentProfile(int id = -1,
                   int student_id = -1,
                   AcademicStanding academic_standing = AcademicStanding::Unknown,
                   double financialBalance = 0.0,
                   std::optional<std::string> phone_number = std::nullopt,
                   std::optional<std::string> emergency_contact_name = std::nullopt,
                   std::optional<std::string> emergency_contact_phone = std::nullopt,
                   std::optional<std::string> address = std::nullopt,
                   std::optional<std::string> city = std::nullopt,
                   std::optional<std::string> state = std::nullopt,
                   std::optional<std::string> postal_code = std::nullopt,
                   std::optional<std::string> country = std::nullopt,
                   std::optional<std::string> medical_notes = std::nullopt
            );
    StudentProfile( const StudentProfile& SP ) = default;
    StudentProfile( StudentProfile&& SP ) = default;
    StudentProfile& operator=( const StudentProfile& SP ) = default;
    StudentProfile& operator=( StudentProfile&& SP ) = default;
    ~StudentProfile() {};

    int getId() const { return m_id; }
    void setId( int id ) { m_id = id; }

    int getStudentId() const { return m_student_id; }
    void setStudentId( int sid ) { m_student_id = sid; }

    AcademicStanding getAcademicStanding() const { return m_academic_standing; }
    void setAcademicStanding( AcademicStanding as ) { m_academic_standing = as; }

    double getFinancialBalance() const { return m_financialBalance; }
    void setFinanceBalance( double val ) { m_financialBalance = val; }

    std::string getPhone() const { return m_phone_number.value_or(std::string()); }
    void setPhone( const std::string& phone ) { if( phone.empty() ) m_phone_number = std::nullopt; else m_phone_number = phone; }

    std::string getEmergencyPerson() const { return m_emergency_contact_name.value_or(std::string()); }
    void setEmergencyPerson( const std::string& name ) { if( name.empty() ) m_emergency_contact_name = std::nullopt; else m_emergency_contact_name = name; }

    std::string getEmergencyPhone() const { return m_emergency_contact_phone.value_or(std::string()); }
    void setEmergencyPhone( const std::string& phone ) { if( phone.empty() ) m_emergency_contact_phone = std::nullopt; else m_emergency_contact_phone = phone; }

    std::string getAddress() const { return m_address.value_or(std::string()); }
    void setAddress( const std::string& addr ) { if( addr.empty() ) m_address = std::nullopt; else m_address = addr; }

    std::string getCity() const { return m_city.value_or( std::string() ); }
    void setCity( const std::string& city ) { if( city.empty() ) m_city = std::nullopt; else m_city = city; }

    std::string getState() const { return m_state.value_or( std::string() ); }
    void setState( const std::string& state ) { if( state.empty() ) m_state = std::nullopt; else m_state = state; }

    std::string getPostalCode() const { return m_postal_code.value_or( std::string() ); }
    void setPostalCode( const std::string& pcode ) { if( pcode.empty() ) m_postal_code = std::nullopt; else m_postal_code = pcode; }

    std::string getCountry() const { return m_country.value_or( std::string() ); }
    void setCountry( const std::string& pcountry ) { if( pcountry.empty() ) m_country = std::nullopt; else m_country = pcountry; }

    std::string getMedNotes() const { return m_medical_notes.value_or( std::string() ); }
    void setMedNotes( const std::string& mnotes ) { if( mnotes.empty() ) m_medical_notes = std::nullopt; else m_medical_notes = mnotes; }

    bool isValid() const { return (m_id > 0 && m_student_id > 0); }

private:
    int m_id;
    int m_student_id;
    AcademicStanding m_academic_standing;
    double m_financialBalance;
    std::optional<std::string> m_phone_number;
    std::optional<std::string> m_emergency_contact_name;
    std::optional<std::string> m_emergency_contact_phone;
    std::optional<std::string> m_address;
    std::optional<std::string> m_city;
    std::optional<std::string> m_state;
    std::optional<std::string> m_postal_code;
    std::optional<std::string> m_country;
    std::optional<std::string> m_medical_notes;
};
