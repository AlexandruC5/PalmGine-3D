#include "Application.h"
#include "Globals.h"
#include "PanelInspector.h"
#include "ModuleImporter.h"
#include "ModuleSceneIntro.h"
#include "CompTransform.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "CompCamera.h"
#include "CompAudioListener.h"
#include "CompAudioSource.h"
#include "CompMovement.h"
#include "CompReverbZone.h"
#include "ImGuizmo-master/ImGuizmo.h"
#include "ModuleInput.h"
#include "GameObject.h"

PanelInspector::PanelInspector() : Panel("Inspector")
{}
PanelInspector::~PanelInspector()
{}

//Function that draws all the console
void PanelInspector::Draw()
{
	ImGui::Text("GameObject Information");
	ImGui::Separator();
	ImGui::Separator();
	GameObject* selected_go = App->scene_intro->GetSelectedGO();
	if (selected_go != nullptr)
	{
		ImGui::Text("Inspector");
		ImGui::Separator();
		ImGui::Separator();
		// GO name
		char name[50];
		strcpy_s(name, 50, selected_go->GetName().c_str());
		//ImGui::Text("Model Name: %s", name.c_str());
		if(ImGui::InputText("Name", name, 50, ImGuiInputTextFlags_AutoSelectAll |ImGuiInputTextFlags_EnterReturnsTrue)) {
			selected_go->SetName(name);
		}
		if (ImGui::Checkbox("Active", &go_active))
		{
			if (selected_go->IsActive())
				selected_go->SetActive(false);
			else
				selected_go->SetActive(true);
		}
		if (selected_go != App->scene_intro->camera) {
			if (ImGui::Button("Delete GameObject"))
			{
				App->scene_intro->quadtree.Remove(selected_go);
				selected_go->PopGameObject();
				selected_go = nullptr;
			}
		}

		// Components
		if (selected_go != nullptr) {
			CompTransform* transform = selected_go->GetCompTransform();
			CompMesh* mesh = selected_go->GetCompMesh();
			CompMaterial* texture = selected_go->GetCompMaterial();
			CompCamera* camera = selected_go->GetCompCamera();
			CompAudioListener* audio_listener = selected_go->GetCompAudioListener();
			CompAudioSource* audio_source = selected_go->GetCompAudioSource();
			CompMovement* comp_movement = selected_go->GetCompMovement();
			CompReverbZone* reverb_zone = selected_go->GetReverbZone();

			if (mesh != nullptr) {
				//SetStatic
				bool isStatic = selected_go->IsStatic();
				if (ImGui::Checkbox("Static", &isStatic)) {
					selected_go->SetStatic(isStatic);
					if (isStatic == true) {
						App->scene_intro->quadtree.Insert(selected_go);
					}
					else {
						App->scene_intro->quadtree.Remove(selected_go);
					}
				}
			}

			if (transform != nullptr)
			{
				//Enable guizmos
				if (App->scene_intro->game_running == false) {
					EnableGuizmos(selected_go);
				}
			}

			if (ImGui::CollapsingHeader("Transform"))
			{
				if (transform != nullptr)
				{

					ImGui::Text("Showing read only information about the mesh transform");
					ImGui::Separator();
					//Position
					ImGui::Text("Position:");
					if (selected_go->IsStatic() == false) {
						float3 pos = transform->GetPosition();
						if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f)) {
							transform->SetPosition(pos);
						}
					}
					else {
						ImGui::Text("[%f]    [%f]    [%f]", transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z);
					}
					//Rotation
					ImGui::Text("Rotation:");
					if (selected_go->IsStatic() == false) {
						float3 rot = transform->GetRotation();
						if (ImGui::DragFloat3("Rotation", (float*)&rot, -pi, pi)) {
							transform->SetRotation(rot);
						}
					}
					else {
						ImGui::Text("[%f]    [%f]    [%f]", transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z);
					}
					//Scale
					ImGui::Text("Scale:");
					if (selected_go->IsStatic() == false) {
						float3 scale = transform->GetScale();
						if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f)) {
							transform->SetScale(scale);
						}
					}
					else {
						ImGui::Text("[%f]    [%f]    [%f]", transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
					}
				}
				else
					LOG("ERROR: COMPONENT TRANSFORM IS NULLPTR on GameObject with name %s", selected_go->GetName().c_str());
			}

			if (audio_listener != nullptr)
			{
				ImGui::CollapsingHeader("Audio Listener");
			}

			if (mesh != nullptr)
			{
				if (ImGui::CollapsingHeader("Renderer Options"))
				{
					ImGui::Checkbox("Wireframe mode", &mesh->wireframe);
					if (texture != nullptr) {
						ImGui::SliderFloat("Alpha tolerance", &mesh->tex_alpha, 0, 1, NULL);
					}
				}
				if (ImGui::CollapsingHeader("Mesh Information"))
				{
					ImGui::Text("Showing read only information about the mesh");
					ImGui::Separator();
					//Mesh Triangles
					ImGui::Text("Mesh triangles: %i", mesh->GetNumVertices() / 3);
					//Mesh Vertices
					ImGui::Text("Mesh vertices: %i", mesh->GetNumVertices());
					//Mesh Indices
					ImGui::Text("Mesh indices: %i", mesh->GetNumIndices());
					//Mesh Normals
					ImGui::Text("Mesh normals: %f", mesh->GetNumNormals());
					//Mesh UVS
					ImGui::Text("Mesh uvs: %f", mesh->GetNumUvs());
				}
			}

			if (camera != nullptr) {
				if (ImGui::CollapsingHeader("Camera"))
				{
					ImGui::Checkbox("Active", &camera->active);
					ImGui::Checkbox("Frustum Culling", &camera->frustum_culling);
					float near_plane_dis = camera->GetNearPlaneDistance();
					if (ImGui::DragFloat("Near Plane", &near_plane_dis, 0.1f, 0.1f, 1000.0f)) {
						camera->SetNearPlaneDistance(near_plane_dis);
					}
					float far_plane_dis = camera->GetFarPlaneDistance();
					if (ImGui::DragFloat("Far Plane", &far_plane_dis, 0.1f, 25.f, 1000.0f)) {
						camera->SetFarPlaneDistance(far_plane_dis);
					}
					float fov = camera->GetFOV();
					if (ImGui::SliderFloat("Field of View", &fov, 1.0f, 179.0f)) {
						camera->SetFOV(fov);
					}
					float aspect_ratio = camera->GetApectRatio();
					if (ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.1f, 0.1f, 1000.0f)) {
						camera->SetAspectRatio(aspect_ratio);
					}
				}
			}
			if (texture != nullptr) {
				if (ImGui::CollapsingHeader("Material")) {
					ImGui::Text("Showing read only information about the mesh material");
					ImGui::Separator();
					ImGui::Text("Texture path: %s", texture->GetTexturePath().c_str());
					ImGui::Text("Texture WIDTH: %i", texture->GetTextureWidth());
					ImGui::Text("Texture HEIGHT: %i", texture->GetTextureHeight());
					ImGui::Image((ImTextureID)texture->GetTextureId(), ImVec2(200, 200));
				}
			}

			if (audio_source != nullptr) {
				if (ImGui::CollapsingHeader("Audio Source")) {
					char text[100];
					strcpy_s(text, 100, audio_source->GetAudioToPlay().c_str());
					if (ImGui::InputText("AudioClip", text, 100, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
						audio_source->SetAudio(text);
					}
					float time_to_swap = audio_source->GetMusicSwapTime();
					if (ImGui::SliderFloat("Time to swap music", &time_to_swap, 1.0f, 30.0f)) {
						audio_source->SetMusicSwapTimer(time_to_swap);
					}
					if (ImGui::Button("Play")) {
						audio_source->PlayAudio();
					}
					ImGui::SameLine();
					if (ImGui::Button("Stop")) {
						audio_source->StopAudio();
					}

					bool muted = audio_source->isMuted();
					if (ImGui::Checkbox("Mute", &muted)) {
						audio_source->SetMuted(muted);
					}

					bool play_on_awake = audio_source->GetPlayOnAwake();
					if (ImGui::Checkbox("Play On Awake", &play_on_awake)) {
						audio_source->SetPlayOnAwake(play_on_awake);
					}

					int priority = audio_source->GetPriority();
					if (ImGui::SliderInt("Priority", &priority, 1, 100)) {
						audio_source->SetPriority(priority);
					}

					float volume = audio_source->GetVolume();
					if (ImGui::SliderFloat("Volume", &volume, 0, 10)) {
						audio_source->SetVolume(volume);
					}

					float pitch = audio_source->GetPitch();
					if (ImGui::SliderFloat("Pitch", &pitch, 0, 15)) {
						audio_source->SetPitch(pitch);
					}
					ImGui::Separator();
					ImGui::Text("Only for 2D tracks:");
					ImGui::Separator();
					int stereo_pan_l = audio_source->GetStereoPanLeft();
					if (ImGui::SliderInt("Pan Left difference", &stereo_pan_l, 0, 100)) {
						audio_source->SetStereoPanLeft(stereo_pan_l);
					}
					int stereo_pan_r = audio_source->GetStereoPanRight();
					if (ImGui::SliderInt("Pan Right difference", &stereo_pan_r, 0, 100)) {
						audio_source->SetStereoPanRight(stereo_pan_r);
					}
				}
			}

			if (comp_movement != nullptr) {
				if (ImGui::CollapsingHeader("Movement")) 
				{
					float pos_a = comp_movement->GetPosA().x;
					if (ImGui::SliderFloat("Position A", &pos_a, -100, 100))
					{
						float3 final_pos = { pos_a, 0, 0 };
						comp_movement->SetPosA(final_pos);
					}
					float pos_b = comp_movement->GetPosB().x;
					if (ImGui::SliderFloat("Position B", &pos_b, -100, 100))
					{
						float3 final_pos = { pos_b, 0, 0 };
						comp_movement->SetPosB(final_pos);
					}
					int vel = comp_movement->GetVel();
					if (ImGui::SliderInt("Velocity", &vel, 1, 50))
					{
						comp_movement->SetVel(vel);
					}
				}
			}

			if (reverb_zone != nullptr) {
				if (ImGui::CollapsingHeader("Reverb Zone")) {
					bool use_sphere = reverb_zone->UsingSphere();
					if (ImGui::Checkbox("Use Sphere", &use_sphere)) {
						reverb_zone->ShouldUseSphere(use_sphere);
					}
					bool use_cube = reverb_zone->UsingCube();
					if (ImGui::Checkbox("Use Cube", &use_cube)) {
						reverb_zone->ShouldUseCube(use_cube);
					}
					float radius = reverb_zone->GetRadius();
					if (ImGui::SliderFloat("Radius", &radius, 1.0f, 50.0f)) {
						reverb_zone->SetRadius(radius);
					}
				}
			}

		}
	}
}

void PanelInspector::EnableGuizmos(GameObject* selected_go) {
	ImGuizmo::Enable(true);

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	//Swap between guizmos mode using scancodes or uibuttons
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE)){
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE)) {
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE)) {
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	float4x4 matrix = selected_go->GetCompTransform()->GetLocalTransform().Transposed();
	CompTransform* transform = selected_go->GetCompTransform();

	ImGuizmo::Manipulate((float*)&App->camera->GetViewMatrix(), (float*)&App->camera->GetProjectionMatrix(), mCurrentGizmoOperation, ImGuizmo::WORLD, (float*)&matrix);

	if (ImGuizmo::IsUsing() && selected_go->IsStatic() == false)
	{
		matrix.Transpose();
		float3 pos = transform->GetPosition();
		Quat rot = transform->GetRotationQuat();
		float3 scale = transform->GetScale();
		matrix.Decompose(pos, rot, scale);
		if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE) {
			transform->SetPosition(pos);
		}
		else if (mCurrentGizmoOperation == ImGuizmo::ROTATE) {
			transform->SetRotationWithQuat(rot);
		}
		else if (mCurrentGizmoOperation == ImGuizmo::SCALE) {
			transform->SetScale(scale);
		}
	}

}