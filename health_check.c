/* Health Check System (C)
 *
 * Console program that collects basic health inputs from the user and
 * evaluates them against defined thresholds, flagging risk indicators.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 64

static void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

static int read_int(const char *prompt, int min_val, int max_val) {
    char buf[128];
    long value;
    char *end;
    for (;;) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            return min_val;
        }
        value = strtol(buf, &end, 10);
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0' && *end != '\n') {
            printf("  Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("  Out of range (%d-%d). Try again.\n", min_val, max_val);
            continue;
        }
        return (int)value;
    }
}

static double read_double(const char *prompt, double min_val, double max_val) {
    char buf[128];
    double value;
    char *end;
    for (;;) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            return min_val;
        }
        value = strtod(buf, &end);
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0' && *end != '\n') {
            printf("  Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("  Out of range (%.1f-%.1f). Try again.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

static char read_choice(const char *prompt) {
    char buf[128];
    for (;;) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            return 'M';
        }
        /* skip leading spaces */
        char *p = buf;
        while (isspace((unsigned char)*p)) p++;
        char c = (char)toupper((unsigned char)*p);
        if (c == 'M' || c == 'F') {
            return c;
        }
        printf("  Please enter M or F.\n");
    }
}

typedef struct {
    int age;
    char gender;
    double height_cm;
    double weight_kg;
    int systolic;
    int diastolic;
    int heart_rate;
    double temperature_c;
    double blood_sugar;
} HealthInput;

static void collect_inputs(HealthInput *h) {
    printf("=== Health Check System ===\n");
    printf("Enter your health details below.\n\n");

    h->age = read_int("Age (years): ", 1, 120);
    h->gender = read_choice("Gender (M/F): ");
    h->height_cm = read_double("Height (cm): ", 50.0, 250.0);
    h->weight_kg = read_double("Weight (kg): ", 20.0, 400.0);
    h->systolic = read_int("Systolic blood pressure (mmHg): ", 60, 260);
    h->diastolic = read_int("Diastolic blood pressure (mmHg): ", 40, 160);
    h->heart_rate = read_int("Resting heart rate (bpm): ", 30, 220);
    h->temperature_c = read_double("Body temperature (C): ", 30.0, 45.0);
    h->blood_sugar = read_double("Fasting blood sugar (mg/dL): ", 40.0, 400.0);
}

static double compute_bmi(const HealthInput *h) {
    double height_m = h->height_cm / 100.0;
    return h->weight_kg / (height_m * height_m);
}

static const char *bmi_category(double bmi) {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25.0) return "Normal";
    if (bmi < 30.0) return "Overweight";
    return "Obese";
}

static const char *bp_category(int sys, int dia) {
    if (sys > 180 || dia > 120) return "Hypertensive crisis";
    if (sys >= 140 || dia >= 90) return "Stage 2 hypertension";
    if (sys >= 130 || dia >= 80) return "Stage 1 hypertension";
    if (sys >= 120 && dia < 80) return "Elevated";
    return "Normal";
}

static const char *hr_category(int hr) {
    if (hr < 60) return "Bradycardia (low)";
    if (hr > 100) return "Tachycardia (high)";
    return "Normal";
}

static const char *temp_category(double t) {
    if (t < 35.0) return "Hypothermia";
    if (t > 38.0) return "Fever (high)";
    if (t > 37.3) return "Low-grade fever";
    return "Normal";
}

static const char *sugar_category(double s) {
    if (s < 70.0) return "Low (hypoglycemia)";
    if (s < 100.0) return "Normal";
    if (s < 126.0) return "Prediabetes";
    return "Diabetes range";
}

static int count_risk_flags(const HealthInput *h, double bmi) {
    int flags = 0;
    if (bmi < 18.5 || bmi >= 25.0) flags++;
    if (h->systolic >= 120 || h->diastolic >= 80) flags++;
    if (h->heart_rate < 60 || h->heart_rate > 100) flags++;
    if (h->temperature_c < 35.0 || h->temperature_c > 37.3) flags++;
    if (h->blood_sugar < 70.0 || h->blood_sugar >= 100.0) flags++;
    return flags;
}

static const char *overall_risk(int flags) {
    if (flags == 0) return "Low";
    if (flags <= 2) return "Moderate";
    return "High";
}

static void print_report(const HealthInput *h) {
    double bmi = compute_bmi(h);
    int flags = count_risk_flags(h, bmi);

    printf("\n========== HEALTH REPORT ==========\n");
    printf("Age:        %d\n", h->age);
    printf("Gender:     %c\n", h->gender);
    printf("BMI:        %.1f (%s)\n", bmi, bmi_category(bmi));
    printf("Blood BP:   %d/%d mmHg (%s)\n", h->systolic, h->diastolic,
           bp_category(h->systolic, h->diastolic));
    printf("Heart rate: %d bpm (%s)\n", h->heart_rate, hr_category(h->heart_rate));
    printf("Temp:       %.1f C (%s)\n", h->temperature_c, temp_category(h->temperature_c));
    printf("Blood sugar:%.0f mg/dL (%s)\n", h->blood_sugar, sugar_category(h->blood_sugar));
    printf("-----------------------------------\n");
    printf("Risk flags: %d\n", flags);
    printf("Overall risk level: %s\n", overall_risk(flags));
    printf("===================================\n");

    if (flags > 0) {
        printf("\nNote: This is a basic screening tool, not a medical diagnosis.\n");
        printf("Consult a healthcare professional for proper evaluation.\n");
    }
}

int main(void) {
    HealthInput h;
    collect_inputs(&h);
    print_report(&h);
    return 0;
}
EOF
echo done