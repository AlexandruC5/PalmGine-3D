#include "Application.h"
#include "Globals.h"
#include "PanelInspector.h"
#include "ModuleImporter.h"
#include "ModuleSceneIntro.h"
#include "CompTransform.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "CompCamera.h"
#include "ImGuizmo-master/ImGuizmo.h"
#include "ModuleInput.h"

PanelInspector::PanelInspector() : Panel("Inspector")
{}
PanelInspector::~PanelInspector()
{}

//Function that draws all the console
void PanelInspector::Draw()
{
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
		
		//DeleteGameObject
		//TODO Solve crash when delete is clicked in this commented code
		//if (selected_go != App->scene_intro->camera) {
		//	ImGui::SameLine();
		//	if (ImGui::Button("Delete")) {
		//		App->scene_intro->quadtree.Remove(selected_go);
		//		delete selected_go;
		//		App->scene_intro->selected_gameObject = nullptr;
		//		selected_go = nullptr;
		//	}
		//}

		// Components
		if (selected_go != nullptr) {
			CompTransform* transform = selected_go->GetCompTransform();
			CompMesh* mesh = selected_go->GetCompMesh();
			CompMaterial* texture = selected_go->GetCompMaterial();
			CompCamera* camera = selected_go->GetCompCamera();

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

			if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				if (transform != nullptr)
				{
					//Enable guizmos
					EnableGuizmos(selected_go);

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

			if (mesh != nullptr)
			{
				if (ImGui::CollapsingHeader("Renderer Options"), ImGuiTreeNodeFlags_DefaultOpen)
				{
					ImGui::Checkbox("Wireframe mode", &mesh->wireframe);
					if (texture != nullptr) {
						ImGui::SliderFloat("Alpha tolerance", &mesh->tex_alpha, 0, 1, NULL);
					}
				}
				if (ImGui::CollapsingHeader("Mesh Information"), ImGuiTreeNodeFlags_DefaultOpen)
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
				if (ImGui::CollapsingHeader("Camera"), ImGuiTreeNodeFlags_DefaultOpen)
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


			// TODO Add mesh info
			/*{
			ImGui::Text("Showing read only information about the mesh");
			ImGui::Separator();
			//Mesh Triangles
			ImGui::Text("Mesh triangles: %i", App->fbx->GetVerticesQuantity()/3);
			//Mesh Vertices
			ImGui::Text("Mesh vertices: %i", App->fbx->GetVerticesQuantity());
			//Mesh Indices
			ImGui::Text("Mesh indices: %i", App->fbx->GetIndicesQuantity());
			//Mesh Normals
			ImGui::Text("Mesh normals: %f", App->fbx->GetNormalsQuantity());
			//Mesh UVS
			ImGui::Text("Mesh uvs: %f", App->fbx->GetUvsQuanity());
		}*/

			if (texture != nullptr) {
				if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
					ImGui::Text("Showing read only information about the mesh material");
					ImGui::Separator();
					ImGui::Text("Texture path: %s", texture->GetTexturePath().c_str());
					ImGui::Text("Texture WIDTH: %i", texture->GetTextureWidth());
					ImGui::Text("Texture HEIGHT: %i", texture->GetTextureHeight());
					ImGui::Image((ImTextureID)texture->GetTextureId(), ImVec2(200, 200));
				}
			}
		}
	}
	//ImGui::End();
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

	float4x4 matrix = selected_go->GetCompTransform()->GetTransformationMatrix().Transposed();
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