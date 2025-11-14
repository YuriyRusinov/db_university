-- =============================================
-- University Management System Database Script
-- 5 Tables with All Relationship Types
-- PostgreSQL 12+
-- =============================================

-- Enable UUID extension
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- Drop existing tables (in correct order for dependencies)
DROP TABLE IF EXISTS Enrollments CASCADE;
DROP TABLE IF EXISTS Student_Profiles CASCADE;
DROP TABLE IF EXISTS Courses CASCADE;
DROP TABLE IF EXISTS Departments CASCADE;
DROP TABLE IF EXISTS Students CASCADE;

-- =============================================
-- Table 1: Departments
-- =============================================
CREATE TABLE Departments (
    id SERIAL PRIMARY KEY,
    uuid UUID DEFAULT gen_random_uuid() UNIQUE NOT NULL,
    department_code VARCHAR(10) UNIQUE NOT NULL,
    department_name VARCHAR(100) NOT NULL,
    chair_person VARCHAR(100),
    office_location VARCHAR(50),
    budget NUMERIC(12,2) DEFAULT 0,
    established_date DATE,
    is_active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- Table 2: Students
-- =============================================
CREATE TABLE Students (
    id SERIAL PRIMARY KEY,
    uuid UUID DEFAULT gen_random_uuid() UNIQUE NOT NULL,
    student_number biginteger UNIQUE NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    middle_name VARCHAR(50),
    last_name VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_of_birth DATE NOT NULL,
    enrollment_date DATE NOT NULL,
    graduation_date DATE,
    status VARCHAR(20) not null DEFAULT 'active' CHECK (status IN ('active', 'graduated', 'transferred', 'withdrawn')),
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- Table 3: Student_Profiles (1-to-1 with Students)
-- =============================================
CREATE TABLE Student_Profiles (
    id SERIAL PRIMARY KEY,
    uuid UUID DEFAULT gen_random_uuid() UNIQUE NOT NULL,
    student_id INTEGER UNIQUE NOT NULL,
    phone_number VARCHAR(20),
    emergency_contact_name VARCHAR(100),
    emergency_contact_phone VARCHAR(20),
    address TEXT,
    city VARCHAR(50),
    state VARCHAR(50),
    postal_code VARCHAR(20),
    country VARCHAR(50) DEFAULT 'USA',
    medical_notes TEXT,
    academic_standing VARCHAR(20) DEFAULT 'good' CHECK (academic_standing IN ('good', 'probation', 'warning', 'suspended', 'graduated')),
    financial_balance NUMERIC(8,2) DEFAULT 0,
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (student_id) REFERENCES Students(id) ON DELETE CASCADE
);

-- =============================================
-- Table 4: Courses (Many-to-Many with Students)
-- =============================================
CREATE TABLE Courses (
    id SERIAL PRIMARY KEY,
    uuid UUID DEFAULT gen_random_uuid() UNIQUE NOT NULL,
    course_code VARCHAR(20) UNIQUE NOT NULL,
    course_name VARCHAR(100) NOT NULL,
    description TEXT,
    credits INTEGER NOT NULL CHECK (credits BETWEEN 1 AND 5),
    department_id INTEGER NOT NULL,
    instructor_name VARCHAR(100),
    max_capacity INTEGER not null check(max_capacity > 0),
    is_active BOOLEAN not null DEFAULT TRUE,
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (department_id) REFERENCES Departments(id)
);

-- =============================================
-- Table 5: Enrollments (Junction table for Many-to-Many)
-- =============================================
CREATE TABLE Enrollments (
    id SERIAL PRIMARY KEY,
    uuid UUID DEFAULT gen_random_uuid() UNIQUE NOT NULL,
    student_id INTEGER NOT NULL,
    course_id INTEGER NOT NULL,
    enrollment_date DATE NOT NULL,
    grade NUMERIC(4,2) CHECK (grade BETWEEN 0 AND 100),
    status VARCHAR(20) not null DEFAULT 'active' CHECK (status IN ('active', 'completed', 'withdrawn', 'dropped')),
    semester VARCHAR(20) NOT NULL,
    credit_hours INTEGER NOT NULL,
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP,
    
    UNIQUE (student_id, course_id, semester),
    FOREIGN KEY (student_id) REFERENCES Students(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES Courses(id) ON DELETE CASCADE
);

-- =============================================
-- Indexes for Performance
-- =============================================

-- Students indexes
CREATE INDEX idx_students_uuid ON Students(uuid);
CREATE INDEX idx_students_number ON Students(student_number);
CREATE INDEX idx_students_email ON Students(email);
CREATE INDEX idx_students_status ON Students(status);
CREATE INDEX idx_students_name ON Students(last_name, first_name);

-- Courses indexes
CREATE INDEX idx_courses_uuid ON Courses(uuid);
CREATE INDEX idx_courses_code ON Courses(course_code);
CREATE INDEX idx_courses_department ON Courses(department_id);
CREATE INDEX idx_courses_active ON Courses(is_active);

-- Enrollments indexes
CREATE INDEX idx_enrollments_uuid ON Enrollments(uuid);
CREATE INDEX idx_enrollments_student ON Enrollments(student_id);
CREATE INDEX idx_enrollments_course ON Enrollments(course_id);
CREATE INDEX idx_enrollments_semester ON Enrollments(semester);
CREATE INDEX idx_enrollments_status ON Enrollments(status);
CREATE INDEX idx_enrollments_grade ON Enrollments(grade);

-- Departments indexes
CREATE INDEX idx_departments_uuid ON Departments(uuid);
CREATE INDEX idx_departments_code ON Departments(department_code);

-- Student_Profiles indexes
CREATE INDEX idx_profiles_uuid ON Student_Profiles(uuid);
CREATE INDEX idx_profiles_student ON Student_Profiles(student_id);

-- =============================================
-- Constraints
-- =============================================

-- Email format constraint
ALTER TABLE Students ADD CONSTRAINT chk_valid_email 
CHECK (email ~* '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$');

-- Phone number constraint (optional)
ALTER TABLE Student_Profiles ADD CONSTRAINT chk_valid_phone 
CHECK (phone_number ~ '^\+?[0-9\s\-\(\)]{10,}$' OR phone_number IS NULL);

-- Date constraints
ALTER TABLE Students ADD CONSTRAINT chk_reasonable_birth_date 
CHECK (date_of_birth > '1900-01-01' AND date_of_birth < CURRENT_DATE - INTERVAL '16 years');

ALTER TABLE Students ADD CONSTRAINT chk_enrollment_date 
CHECK (enrollment_date >= '2000-01-01' AND enrollment_date <= CURRENT_DATE + INTERVAL '1 year');

-- =============================================
-- Functions
-- =============================================

-- Function to get student full name
CREATE OR REPLACE FUNCTION get_student_full_name(
    first_name VARCHAR, 
    middle_name VARCHAR, 
    last_name VARCHAR
) RETURNS VARCHAR AS $$
BEGIN
    IF middle_name IS NULL OR middle_name = '' THEN
        RETURN first_name || ' ' || last_name;
    ELSE
        RETURN first_name || ' ' || middle_name || ' ' || last_name;
    END IF;
END;
$$ LANGUAGE plpgsql;

-- Function to calculate student age
CREATE OR REPLACE FUNCTION calculate_student_age(birth_date DATE) 
RETURNS INTEGER AS $$
BEGIN
    RETURN DATE_PART('year', AGE(birth_date));
END;
$$ LANGUAGE plpgsql;

-- Function to automatically set updated_at timestamp
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- =============================================
-- Triggers
-- =============================================

-- Update timestamps triggers
CREATE TRIGGER update_students_updated_at 
    BEFORE UPDATE ON Students 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE OR REPLACE FUNCTION generate_student_number()
RETURNS TRIGGER AS $$
DECLARE
    year_part VARCHAR(4);
    sequence_num INTEGER;
    new_student_number VARCHAR(20);
BEGIN
    -- Extract year from enrollment date
    year_part := TO_CHAR(NEW.enrollment_date, 'YYYY');
    
    -- Get next sequence number for this year
    SELECT COALESCE(MAX(CAST(SUBSTRING(student_number FROM 5) AS INTEGER)), 0) + 1
    INTO sequence_num
    FROM Students
    WHERE student_number LIKE year_part || '%';
    
    -- Format: YYYY00001
    new_student_number := year_part || LPAD(sequence_num::TEXT, 5, '0');
    
    -- Set the student number
    NEW.student_number := new_student_number;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

create trigger trg_generate_student_number before insert on 
    students FOR EACH ROW
    EXECUTE FUNCTION generate_student_number();

CREATE TRIGGER update_courses_updated_at 
    BEFORE UPDATE ON Courses 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_enrollments_updated_at 
    BEFORE UPDATE ON Enrollments 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_departments_updated_at 
    BEFORE UPDATE ON Departments 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_student_profiles_updated_at 
    BEFORE UPDATE ON Student_Profiles 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

-- =============================================
-- Verification
-- =============================================

-- Verify we have exactly 5 tables
SELECT 'Tables created: ' || COUNT(*) 
FROM information_schema.tables 
WHERE table_schema = 'public' 
AND table_type = 'BASE TABLE';

-- Verify relationships work
SELECT 
    '1:1 Relationship: ' || COUNT(*) || ' student profiles' as relationship_check
FROM Student_Profiles sp
INNER JOIN Students s ON sp.student_id = s.id;

SELECT 
    '1:Many Relationship: ' || COUNT(*) || ' courses per department' as relationship_check
FROM Courses c
INNER JOIN Departments d ON c.department_id = d.id
GROUP BY d.department_name
ORDER BY COUNT(*) DESC
LIMIT 1;

SELECT 
    'Many:Many Relationship: ' || COUNT(DISTINCT e.student_id) || ' students in multiple courses' as relationship_check
FROM Enrollments e
GROUP BY e.course_id
ORDER BY COUNT(*) DESC
LIMIT 1;

-- =============================================
-- Database Creation Complete - 5 Tables
-- =============================================
