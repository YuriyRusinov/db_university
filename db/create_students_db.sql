-- Drop tables if they exist (in correct order due to dependencies)
DROP TABLE IF EXISTS Assignments CASCADE;
DROP TABLE IF EXISTS Enrollments CASCADE;
DROP TABLE IF EXISTS StudentProfiles CASCADE;
DROP TABLE IF EXISTS Courses CASCADE;
DROP TABLE IF EXISTS Students CASCADE;

-- Create Students table
CREATE TABLE Students (
    student_id SERIAL PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_of_birth DATE NOT NULL,
    enrollment_date DATE NOT NULL
);

-- Create StudentProfiles table (1-to-1)
CREATE TABLE StudentProfiles (
    profile_id SERIAL PRIMARY KEY,
    student_id INTEGER UNIQUE NOT NULL,
    address VARCHAR(200),
    phone_number VARCHAR(20),
    emergency_contact VARCHAR(100),
    blood_type VARCHAR(5),
    CONSTRAINT fk_student
        FOREIGN KEY (student_id) 
        REFERENCES Students(student_id) 
        ON DELETE CASCADE
);

-- Create Courses table
CREATE TABLE Courses (
    course_id SERIAL PRIMARY KEY,
    course_code VARCHAR(10) UNIQUE NOT NULL,
    course_name VARCHAR(100) NOT NULL,
    credits INTEGER NOT NULL,
    department VARCHAR(50) NOT NULL
);

-- Create Enrollments table (Many-to-Many)
CREATE TABLE Enrollments (
    enrollment_id SERIAL PRIMARY KEY,
    student_id INTEGER NOT NULL,
    course_id INTEGER NOT NULL,
    enrollment_date DATE NOT NULL,
    grade VARCHAR(2),
    semester VARCHAR(20) NOT NULL,
    CONSTRAINT fk_enrollment_student
        FOREIGN KEY (student_id) 
        REFERENCES Students(student_id) 
        ON DELETE CASCADE,
    CONSTRAINT fk_enrollment_course
        FOREIGN KEY (course_id) 
        REFERENCES Courses(course_id) 
        ON DELETE CASCADE,
    CONSTRAINT unique_enrollment 
        UNIQUE (student_id, course_id, semester)
);

-- Create Assignments table (1-to-Many)
CREATE TABLE Assignments (
    assignment_id SERIAL PRIMARY KEY,
    course_id INTEGER NOT NULL,
    assignment_name VARCHAR(100) NOT NULL,
    description TEXT,
    due_date DATE NOT NULL,
    max_points INTEGER NOT NULL,
    CONSTRAINT fk_assignment_course
        FOREIGN KEY (course_id) 
        REFERENCES Courses(course_id) 
        ON DELETE CASCADE
);

-- Create indexes for better query performance
CREATE INDEX idx_studentprofiles_student_id ON StudentProfiles(student_id);
CREATE INDEX idx_enrollments_student_id ON Enrollments(student_id);
CREATE INDEX idx_enrollments_course_id ON Enrollments(course_id);
CREATE INDEX idx_assignments_course_id ON Assignments(course_id);
